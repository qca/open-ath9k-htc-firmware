/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *  * Neither the name of Qualcomm Atheros nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "usb_defs.h"
#include "usb_type.h"
#include "usb_pre.h"
#include "usb_extr.h"
#include "usb_std.h"
#include "reg_defs.h"
#include "athos_api.h"
#include "usbfifo_api.h"

#include "sys_cfg.h"

#define measure_time 0
#define measure_time_pll 10000000

typedef void (* USBFIFO_recv_command)(VBUF *cmd);

extern Action eUsbCxFinishAction;
extern CommandType eUsbCxCommand;
extern BOOLEAN UsbChirpFinish;
extern USB_FIFO_CONFIG usbFifoConf;
extern uint16_t *pu8DescriptorEX;
extern uint16_t u16TxRxCounter;

USBFIFO_recv_command m_origUsbfifoRecvCmd = NULL;

void zfTurnOffPower_patch(void);

static void _fw_reset_dma_fifo();
static void _fw_restore_dma_fifo();
static void _fw_power_on();
static void _fw_power_off();

BOOLEAN bEepromExist = TRUE;
BOOLEAN bJumptoFlash = FALSE;

void _fw_usbfifo_recv_command(VBUF *buf)
{
	A_UINT8 *cmd_data;
	A_UINT32 tmp;

	cmd_data = (A_UINT8 *)(buf->desc_list->buf_addr + buf->desc_list->data_offset);
	tmp = *((A_UINT32 *)cmd_data);
	if ( tmp == 0xFFFFFFFF ) {	
		// reset usb/wlan dma
		_fw_reset_dma_fifo();

		// restore gpio setting and usb/wlan dma state
		_fw_restore_dma_fifo();

		// set clock to bypass mode - 40Mhz from XTAL 
		HAL_WORD_REG_WRITE(MAGPIE_REG_CPU_PLL_BYPASS_ADDR, (BIT0|BIT4));

		A_DELAY_USECS(100); // wait for stable

		HAL_WORD_REG_WRITE(MAGPIE_REG_CPU_PLL_ADDR, (BIT16));

		A_DELAY_USECS(100); // wait for stable
		A_UART_HWINIT((40*1000*1000), 19200);

		A_CLOCK_INIT(40);

		if (!bEepromExist) { //jump to flash boot (eeprom data in flash)
			bJumptoFlash = TRUE;
			A_PRINTF("Jump to Flash BOOT\n");
			app_start();
		}else{
			A_PRINTF("receive the suspend command...\n");
			// reboot.....
			A_USB_JUMP_BOOT();	        
		}

	} else {
		m_origUsbfifoRecvCmd(buf);
	}
}

void _fw_usbfifo_init(USB_FIFO_CONFIG *pConfig)
{
	m_origUsbfifoRecvCmd = pConfig->recv_command;

	usbFifoConf.get_command_buf = pConfig->get_command_buf;
	usbFifoConf.recv_command    = _fw_usbfifo_recv_command;
	usbFifoConf.get_event_buf   = pConfig->get_event_buf;
	usbFifoConf.send_event_done = pConfig->send_event_done;
}

/*
 * -- support more than 64 bytes command on ep4 -- 
 */
void vUsb_Reg_Out_patch(void)
{   
	uint16_t usbfifolen;
	uint16_t ii;
	uint32_t ep4_data;
	static volatile uint32_t *regaddr;
	static uint16_t cmdLen;
	static VBUF *buf;
	BOOLEAN cmd_is_last = FALSE;
	static BOOLEAN cmd_is_new = TRUE;

	// get the size of this transcation
	usbfifolen = USB_BYTE_REG_READ(ZM_EP4_BYTE_COUNT_LOW_OFFSET);

	// check is command is new
	if( cmd_is_new ){

		buf = usbFifoConf.get_command_buf();
		cmdLen = 0;

		if( !buf )
			goto ERR;

		// copy free, assignment buffer of the address
		regaddr = (uint32_t *)buf->desc_list->buf_addr;

		cmd_is_new = FALSE;
	}

	// just in case, suppose should not happen
	if( !buf )
		goto ERR;

	// if size is smaller, this is the last command!
	// zero-length supposed should be set through 0x27/bit7->0x19/bit4, not here
	if( usbfifolen<64 ) {
		cmd_is_last = TRUE;
	}

	// accumulate the size
	cmdLen += usbfifolen;

	// round it to alignment
	if(usbfifolen % 4)
		usbfifolen = (usbfifolen >> 2) + 1;
	else
		usbfifolen = usbfifolen >> 2;

	// retrieve the data from fifo
	for(ii = 0; ii < usbfifolen; ii++)
	{
		ep4_data = USB_WORD_REG_READ(ZM_EP4_DATA_OFFSET);   // read fifo data out
		*regaddr = ep4_data;
		regaddr++;
	}

	// if this is the last command, callback to HTC
	if (  cmd_is_last  )
	{
		buf->desc_list->next_desc = NULL;
		buf->desc_list->data_offset = 0;
		buf->desc_list->data_size = cmdLen;
		buf->desc_list->control = 0;
		buf->next_buf = NULL;
		buf->buf_length = cmdLen;

		usbFifoConf.recv_command(buf);

		cmd_is_new = TRUE;
	}

	goto DONE;
ERR:
	//    we might get no command buffer here?
	//    but if we return here, the ep4 fifo will be lock out,
	//    so that we still read them out but just drop it ?
	for(ii = 0; ii < usbfifolen; ii++)
	{
		ep4_data = USB_WORD_REG_READ(ZM_EP4_DATA_OFFSET);   // read fifo data out
	}

DONE:
	//mUSB_STATUS_IN_INT_ENABLE();
	;
}


/* 
 *  -- usb1.1 ep6 fix --
 */
extern uint16_t       u8UsbConfigValue;
extern uint16_t       u8UsbInterfaceValue;
extern uint16_t       u8UsbInterfaceAlternateSetting;
extern SetupPacket    ControlCmd;
extern void vUsbClrEPx(void);

#undef FS_C1_I0_A0_EP_NUMBER
#define FS_C1_I0_A0_EP_NUMBER 6

#define FS_C1_I0_A0_EP6_BLKSIZE    BLK512BYTE
#define FS_C1_I0_A0_EP6_BLKNO      DOUBLE_BLK
#define FS_C1_I0_A0_EP6_DIRECTION  DIRECTION_OUT
#define FS_C1_I0_A0_EP6_TYPE       TF_TYPE_BULK
#define FS_C1_I0_A0_EP6_MAX_PACKET 0x0040
#define FS_C1_I0_A0_EP6_bInterval  00

// EP6
#define FS_C1_I0_A0_EP6_FIFO_START  (FS_C1_I0_A0_EP5_FIFO_START + FS_C1_I0_A0_EP5_FIFO_NO)
#define FS_C1_I0_A0_EP6_FIFO_NO     (FS_C1_I0_A0_EP6_BLKNO * FS_C1_I0_A0_EP6_BLKSIZE)
#define FS_C1_I0_A0_EP6_FIFO_CONFIG (0x80 | ((FS_C1_I0_A0_EP6_BLKSIZE - 1) << 4) | ((FS_C1_I0_A0_EP6_BLKNO - 1) << 2) | FS_C1_I0_A0_EP6_TYPE)
#define FS_C1_I0_A0_EP6_FIFO_MAP    (((1 - FS_C1_I0_A0_EP6_DIRECTION) << 4) | EP6)
#define FS_C1_I0_A0_EP6_MAP         (FS_C1_I0_A0_EP6_FIFO_START |   (FS_C1_I0_A0_EP6_FIFO_START << 4)   | (MASK_F0 >> (4*FS_C1_I0_A0_EP6_DIRECTION)))


#define CMD_PCI_RC_RESET_ON() HAL_WORD_REG_WRITE(MAGPIE_REG_RST_RESET_ADDR, \
			 (HAL_WORD_REG_READ(MAGPIE_REG_RST_RESET_ADDR)|	\
			  (PCI_RC_PHY_SHIFT_RESET_BIT|PCI_RC_PLL_RESET_BIT|PCI_RC_PHY_RESET_BIT|PCI_RC_RESET_BIT)))

void vUSBFIFO_EP6Cfg_FS_patch(void)
{
#if (FS_C1_I0_A0_EP_NUMBER >= 6)
	int i;

	//EP0X06
	mUsbEPMap(EP6, FS_C1_I0_A0_EP6_MAP);
	mUsbFIFOMap(FS_C1_I0_A0_EP6_FIFO_START, FS_C1_I0_A0_EP6_FIFO_MAP);
	mUsbFIFOConfig(FS_C1_I0_A0_EP6_FIFO_START, FS_C1_I0_A0_EP6_FIFO_CONFIG);

	for(i = FS_C1_I0_A0_EP6_FIFO_START + 1 ;
            i < FS_C1_I0_A0_EP6_FIFO_START + FS_C1_I0_A0_EP6_FIFO_NO ; i ++)
	{
		mUsbFIFOConfig(i, (FS_C1_I0_A0_EP6_FIFO_CONFIG & (~BIT7)) );
	}

	mUsbEPMxPtSzHigh(EP6, FS_C1_I0_A0_EP6_DIRECTION, (FS_C1_I0_A0_EP6_MAX_PACKET & 0x7ff));
	mUsbEPMxPtSzLow(EP6, FS_C1_I0_A0_EP6_DIRECTION, (FS_C1_I0_A0_EP6_MAX_PACKET & 0x7ff));
	mUsbEPinHighBandSet(EP6 , FS_C1_I0_A0_EP6_DIRECTION, FS_C1_I0_A0_EP6_MAX_PACKET);
#endif
}

void vUsbFIFO_EPxCfg_FS_patch(void)
{
	switch (u8UsbConfigValue)
	{
#if (FS_CONFIGURATION_NUMBER >= 1)
		// Configuration 0X01
        case 0X01:
		switch (u8UsbInterfaceValue)
		{
#if (FS_C1_INTERFACE_NUMBER >= 1)
			// Interface 0
                case 0:
			switch (u8UsbInterfaceAlternateSetting)
			{

#if (FS_C1_I0_ALT_NUMBER >= 1)
				// AlternateSetting 0
                        case 0:

				// snapped....

				// patch up this ep6_fs config
				vUSBFIFO_EP6Cfg_FS_patch();

				break;

#endif
                        default:
				break;
			}
			break;
#endif
                default:
			break;
		}
		break;
#endif
        default:
		break;
	}
	//mCHECK_STACK();
}


BOOLEAN bSet_configuration_patch(void)
{
	bSet_configuration();

	if (mLOW_BYTE(mDEV_REQ_VALUE()) == 0)
	{
		// snapped....
		;
	}
	else
	{
		if (mUsbHighSpeedST())                  // First judge HS or FS??
		{
			// snapped....
			;
		}
		else
		{
			// snapped....
			vUsbFIFO_EPxCfg_FS_patch();
		}

		// snapped....
	}

	eUsbCxFinishAction = ACT_DONE;
	return TRUE;
}


/*
 * -- support more than 64 bytes command on ep3 -- 
 */
void vUsb_Status_In_patch(void)
{
	uint16_t count;
	uint16_t remainder;
	u16_t RegBufLen;
	BOOLEAN cmdEnd = FALSE;

	static u16_t mBufLen;
	static VBUF *evntbuf = NULL;
	static volatile u32_t *regaddr;
	static BOOLEAN cmd_is_new = TRUE;

	if( cmd_is_new )
	{
		evntbuf = usbFifoConf.get_event_buf();
		if ( evntbuf != NULL )
		{
			regaddr = (u32_t *)VBUF_GET_DATA_ADDR(evntbuf);
			mBufLen = evntbuf->buf_length;
		}
		else
		{
			mUSB_STATUS_IN_INT_DISABLE();
			goto ERR_DONE;
		}

		cmd_is_new = FALSE;
	}

	if( mBufLen > bUSB_EP_MAX_PKT_SIZE_64 ) {
		RegBufLen = bUSB_EP_MAX_PKT_SIZE_64;
		mBufLen -= bUSB_EP_MAX_PKT_SIZE_64;
	}
	// TODO: 64 byes... controller supposed will take care of zero-length?
	else {
		RegBufLen = mBufLen;
		cmdEnd = TRUE;
	}

	/* INT use EP3 */
	for(count = 0; count < (RegBufLen / 4); count++)
	{
		USB_WORD_REG_WRITE(ZM_EP3_DATA_OFFSET, *regaddr);
		regaddr++;
	}

	remainder = RegBufLen % 4;

	if (remainder)
	{
		switch(remainder)
		{
		case 3:
			USB_WORD_REG_WRITE(ZM_CBUS_FIFO_SIZE_OFFSET, 0x7);
			break;
		case 2:
			USB_WORD_REG_WRITE(ZM_CBUS_FIFO_SIZE_OFFSET, 0x3);
			break;
		case 1:
			USB_WORD_REG_WRITE(ZM_CBUS_FIFO_SIZE_OFFSET, 0x1);
			break;
		}

		USB_WORD_REG_WRITE(ZM_EP3_DATA_OFFSET, *regaddr);

		// Restore CBus FIFO size to word size            
		USB_WORD_REG_WRITE(ZM_CBUS_FIFO_SIZE_OFFSET, 0xF);
	}

	mUSB_EP3_XFER_DONE();

	if ( evntbuf != NULL && cmdEnd )
	{
		usbFifoConf.send_event_done(evntbuf);
		cmd_is_new = TRUE;
	}

ERR_DONE:
	;
}


#define PCI_RC_RESET_BIT                            BIT6
#define PCI_RC_PHY_RESET_BIT                        BIT7
#define PCI_RC_PLL_RESET_BIT                        BIT8
#define PCI_RC_PHY_SHIFT_RESET_BIT                  BIT10


/*
 * -- urn_off_merlin --
 * . values suggested from Lalit
 *
 */
static void turn_off_merlin()
{
	volatile uint32_t default_data[9];
	uint32_t i=0;

	if(1)
	{
		A_PRINTF("turn_off_merlin_ep_start ......\n");
		A_DELAY_USECS(measure_time);
		default_data[0] = 0x9248fd00;
		default_data[1] = 0x24924924;
		default_data[2] = 0xa8000019;
		default_data[3] = 0x17160820;
		default_data[4] = 0x25980560;
		default_data[5] = 0xc1c00000;
		default_data[6] = 0x1aaabe40;
		default_data[7] = 0xbe105554;
		default_data[8] = 0x00043007;
        
		for(i=0; i<9; i++)
		{
			A_DELAY_USECS(10);
        
			HAL_WORD_REG_WRITE( 0x10ff4040, default_data[i]); 
		}
		A_DELAY_USECS(10);
		HAL_WORD_REG_WRITE(0x10ff4044, BIT0);
		A_PRINTF("turn_off_merlin_ep_end ......\n");
	}
}

/*
 * -- turn_off_phy --
 *
 * . write shift register to both pcie ep and rc
 * . 
 */

static void turn_off_phy()
{

	volatile uint32_t default_data[9];
	volatile uint32_t read_data = 0;
	uint32_t i=0;

	default_data[0] = 0x9248fd00;
	default_data[1] = 0x24924924;
	default_data[2] = 0xa8000019;
	default_data[3] = 0x17160820;
	default_data[4] = 0x25980560;
	default_data[5] = 0xc1c00000;
	default_data[6] = 0x1aaabe40;
	default_data[7] = 0xbe105554;
	default_data[8] = 0x00043007;

	for(i=0; i<9; i++)
	{
		// check for the done bit to be set 

		while (1)
		{
			read_data=HAL_WORD_REG_READ(0x40028);
			if( read_data & BIT31 )
				break;
		}
        
		A_DELAY_USECS(1);
    
		HAL_WORD_REG_WRITE( 0x40024, default_data[i]); 
	}
	HAL_WORD_REG_WRITE(0x40028, BIT0);
}

static void turn_off_phy_rc()
{
    
	volatile uint32_t default_data[9];
	volatile uint32_t read_data = 0;
	uint32_t i=0;
    
	A_PRINTF("turn_off_phy_rc\n");
    
	default_data[0] = 0x9248fd00;
	default_data[1] = 0x24924924;
	default_data[2] = 0xa8000019;
	default_data[3] = 0x13160820;//PwdClk1MHz=0
	default_data[4] = 0x25980560;
	default_data[5] = 0xc1c00000;
	default_data[6] = 0x1aaabe40;
	default_data[7] = 0xbe105554;
	default_data[8] = 0x00043007;
        
	for(i=0; i<9; i++)
	{
		// check for the done bit to be set 
     
		while (1)
		{
			read_data=HAL_WORD_REG_READ(0x40028);
			if( read_data & BIT31 )
				break;
		}

		A_DELAY_USECS(1);

		HAL_WORD_REG_WRITE( 0x40024, default_data[i]); 
	}
	HAL_WORD_REG_WRITE(0x40028, BIT0);
}

volatile uint32_t gpio_func = 0x0;
volatile uint32_t gpio = 0x0;

/*
 * -- patch zfTurnOffPower --
 *
 * . set suspend counter to non-zero value
 * . 
 */
void zfTurnOffPower_patch(void)
{
	A_PRINTF("+++ goto suspend ......\n");

	// setting the go suspend here, power down right away...
	HAL_WORD_REG_WRITE(0x10000, HAL_WORD_REG_READ(0x10000)|(0x8));

	A_DELAY_USECS(100);

	// TURN OFF ETH PLL
	_fw_power_off();

	//32clk wait for External ETH PLL stable
	A_DELAY_USECS(100);
    
	HAL_WORD_REG_WRITE(0x52000, 0x70303);//read back 0x703f7
	HAL_WORD_REG_WRITE(0x52008, 0x0e91c);//read back 0x1e948    
    
	HAL_WORD_REG_WRITE(MAGPIE_REG_SUSPEND_ENABLE_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_SUSPEND_ENABLE_ADDR)|(BIT0))); //0x56030
	// wake up, and turn on cpu, eth, pcie and usb pll 
	_fw_power_on();
	// restore gpio and other settings
	_fw_restore_dma_fifo();

	// clear suspend..................
	HAL_WORD_REG_WRITE(MAGPIE_REG_SUSPEND_ENABLE_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_SUSPEND_ENABLE_ADDR)&(~BIT0)));
	HAL_WORD_REG_WRITE(0x52028, HAL_WORD_REG_READ(0x52028)&(~(BIT8|BIT12|BIT16)));
}

/*
 * -- patch zfResetUSBFIFO_patch --
 *
 * . clear ep3/ep4 fifo
 * . set suspend magic pattern
 * . reset pcie ep phy
 * . reset pcie rc phy
 * . turn off pcie pll
 * . reset all pcie/gmac related registers
 * . reset usb dma
 */
void zfResetUSBFIFO_patch(void)
{
	A_PRINTF("0x9808  0x%x ......\n", HAL_WORD_REG_READ(0x10ff9808));
	A_PRINTF("0x7890  0x%x ......\n", HAL_WORD_REG_READ(0x10ff7890));
	A_PRINTF("0x7890  0x%x ......\n", HAL_WORD_REG_READ(0x10ff7890));
	A_PRINTF("0x4088  0x%x ......\n", HAL_WORD_REG_READ(0x10ff4088));
	_fw_reset_dma_fifo();
}

static void _fw_reset_dma_fifo()
{
	HAL_BYTE_REG_WRITE(0x100ae, (HAL_BYTE_REG_READ(0x100ae)|0x10));
	HAL_BYTE_REG_WRITE(0x100ae, (HAL_BYTE_REG_READ(0x100af)|0x10));
	A_PRINTF("_fw_reset_dma_fifo\n");

	// disable ep3 int enable, so that resume back won't send wdt magic pattern out!!!
	mUSB_STATUS_IN_INT_DISABLE();

	// update magic pattern to indicate this is a suspend
	HAL_WORD_REG_WRITE(WATCH_DOG_MAGIC_PATTERN_ADDR, SUS_MAGIC_PATTERN);

	A_PRINTF("org 0x4048  0x%x ......\n", HAL_WORD_REG_READ(0x10ff4048));
	A_PRINTF("org 0x404C  0x%x ......\n", HAL_WORD_REG_READ(0x10ff404C));
	A_PRINTF("org 0x4088  0x%x ......\n", HAL_WORD_REG_READ(0x10ff4088));

	HAL_WORD_REG_WRITE(0x10ff4088,0xaaa6a);//1010.1010.1010.0110.1010 for UB94
	HAL_WORD_REG_WRITE(0x10ff404C,0x0);

	A_DELAY_USECS(1000);
	A_PRINTF("0x4048  0x%x ......\n", HAL_WORD_REG_READ(0x10ff4048));
	A_PRINTF("0x404C  0x%x ......\n", HAL_WORD_REG_READ(0x10ff404C));
	A_PRINTF("0x4088  0x%x ......\n", HAL_WORD_REG_READ(0x10ff4088));
         
	// turn off merlin
	turn_off_merlin();
	// pcie ep
	A_PRINTF("turn_off_magpie_ep_start ......\n");
	A_DELAY_USECS(measure_time);
	HAL_WORD_REG_WRITE( 0x40040, (HAL_WORD_REG_READ(0x40040)|BIT0|(1<<1)));
	turn_off_phy();
	HAL_WORD_REG_WRITE( 0x40040, (HAL_WORD_REG_READ(0x40040)&~(BIT0|(1<<1))));
	A_PRINTF("turn_off_magpie_ep_end ......\n");

	// pcie rc 
	A_PRINTF("turn_off_magpie_rc_start ......\n");
	A_DELAY_USECS(measure_time);
	HAL_WORD_REG_WRITE( 0x40040, (HAL_WORD_REG_READ(0x40040)&(~BIT0)));
	turn_off_phy_rc();
	A_PRINTF("turn_off_magpie_rc_end ......down\n");
	A_DELAY_USECS(measure_time);

	A_PRINTF("0x4001C  %p ......\n", HAL_WORD_REG_READ(0x4001c)); 
	A_PRINTF("0x40040  %p ......\n", HAL_WORD_REG_READ(0x40040));
    
	// turn off pcie_pll - power down (bit16)
	A_PRINTF(" before pwd PCIE PLL CFG:0x5601C  %p ......\n", HAL_WORD_REG_READ(0x5601C));
	HAL_WORD_REG_WRITE(0x5601C, (HAL_WORD_REG_READ(0x5601C)|(BIT18)));   
	A_PRINTF(" after pwd PCIE PLL CFG:0x5601C  %p ......\n", HAL_WORD_REG_READ(0x5601C));

	/* set everything to reset state?, requested by Oligo */
	HAL_WORD_REG_WRITE(0x50010, HAL_WORD_REG_READ(0x50010)|(BIT13|BIT12|BIT11|BIT9|BIT7|BIT6));

	HAL_WORD_REG_WRITE(0x5C000, 0);

	A_DELAY_USECS(10);

	// reset usb DMA controller
	HAL_WORD_REG_WRITE((USB_CTRL_BASE_ADDRESS+0x118), 0x0);

	HAL_WORD_REG_WRITE(0x50010, (HAL_WORD_REG_READ(0x50010)|(BIT4)));
	A_DELAY_USECS(5);
	HAL_WORD_REG_WRITE(0x50010, (HAL_WORD_REG_READ(0x50010)&(~BIT4)));


	HAL_WORD_REG_WRITE((USB_CTRL_BASE_ADDRESS+0x118), 0x1);
}

static void _fw_power_off()
{
	/*
	 *  1. set CPU bypass
	 *  2. turn off CPU PLL
	 *  3. turn off ETH PLL
	 *  4. disable ETH PLL bypass and update
	 *  4.1 set suspend timeout 
	 *  5. set SUSPEND_ENABLE
	 */

	HAL_WORD_REG_WRITE(MAGPIE_REG_CPU_PLL_BYPASS_ADDR, (BIT0|BIT4)); //0x56004

	A_DELAY_USECS(100); // wait for stable

	HAL_WORD_REG_WRITE(MAGPIE_REG_CPU_PLL_ADDR, (BIT16));//0x56000

	A_DELAY_USECS(100); // wait for stable

	A_UART_HWINIT((40*1000*1000), 19200);
	A_CLOCK_INIT(40);

	HAL_WORD_REG_WRITE(MAGPIE_REG_ETH_PLL_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_ETH_PLL_ADDR)|(BIT16)));   //0x5600c

	HAL_WORD_REG_WRITE(MAGPIE_REG_ETH_PLL_BYPASS_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_ETH_PLL_BYPASS_ADDR)|(BIT4|BIT0))); //0x56010

	HAL_WORD_REG_WRITE(MAGPIE_REG_SUSPEND_ENABLE_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_SUSPEND_ENABLE_ADDR)|(0x10<<8))); //0x56030
}

static void _fw_power_on()
{ 
    /*
     *  1. turn on CPU PLL
     *  2. disable CPU bypass
     *  3. turn on ETH PLL
     *  4. disable ETH PLL bypass and update
     *  5. turn on pcie pll
     */    

	HAL_WORD_REG_WRITE(MAGPIE_REG_ETH_PLL_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_ETH_PLL_ADDR)&(~BIT16)));

	// deassert eth_pll bypass mode and trigger update bit
	HAL_WORD_REG_WRITE(MAGPIE_REG_ETH_PLL_BYPASS_ADDR,
			   (HAL_WORD_REG_READ(MAGPIE_REG_ETH_PLL_BYPASS_ADDR)&(~(BIT4|BIT0))));
}

static void _fw_restore_dma_fifo(void)
{
	HAL_WORD_REG_WRITE(0x5601C, (HAL_WORD_REG_READ(0x5601C)&(~(BIT18))));
    
	// reset pcie_rc shift 
	HAL_WORD_REG_WRITE(0x50010, (HAL_WORD_REG_READ(0x50010)&(~(BIT10|BIT8|BIT7))));
	A_DELAY_USECS(1);
	HAL_WORD_REG_WRITE(0x50010, (HAL_WORD_REG_READ(0x50010)|(BIT10|BIT8|BIT7)));

	// reset pci_rc phy
	CMD_PCI_RC_RESET_ON();
	A_DELAY_USECS(20);

	// enable dma swap function
	MAGPIE_REG_USB_RX0_SWAP_DATA = 0x1;
	MAGPIE_REG_USB_TX0_SWAP_DATA = 0x1;
	MAGPIE_REG_USB_RX1_SWAP_DATA = 0x1;
	MAGPIE_REG_USB_RX2_SWAP_DATA = 0x1;
}

extern uint16_t *u8ConfigDescriptorEX;
extern uint16_t *pu8DescriptorEX;
extern uint16_t u16TxRxCounter;

extern uint16_t *u8UsbDeviceDescriptor;

extern BOOLEAN bGet_descriptor(void);

uint16_t ConfigDescriptorPatch[30];

uint16_t UsbDeviceDescriptorPatch[9];
#define BCD_DEVICE_OFFSET		6
#define BCD_DEVICE_FW_SIGNATURE		0xffff
#define VENDOR_ID_OFFSET		4
#define PRODUCT_ID_OFFSET		5

#define EP3_TRANSFER_TYPE_OFFSET    17
#define EP3_INT_INTERVAL            19
#define EP4_TRANSFER_TYPE_OFFSET    21
#define EP4_INT_INTERVAL            22



 #define A_SFLASH_READ_4B(u32Data, start_addr) u32Data = *(uint32_t *)(0xf000000+start_addr);
 #define FLASH_SIZE 0x800000 //8M
 #define FLASH_USB_VENDOR_ID_OFFSET     0x86
 #define FLASH_USB_PRODUCT_ID_OFFSET    0x87

 // flash reserved size for saving eeprom data is 4K.
 #define EE_DATA_RESERVED_LEN 0x1000 //4K

#define mLOW_MASK(u16)          ((uint8_t) ((u16) & mMASK(8)))
#define mHIGH_MASK(u16)         ((uint8_t) ((u16) & ~mMASK(8)))

/* (1234) -> 0034 */
//#define mLOW_BYTE(u16)          ((U_8)(u16))
#define mLOW_BYTE(u16)          mLOW_MASK(u16)
/* (1234) -> 0012 */
#define mHIGH_BYTE(u16)         ((uint8_t) (((uint16_t) (u16)) >> 8))

#define mLOW_WORD0(u32)         ((uint16_t) ((u32) & 0xFFFF))
#define mHIGH_WORD0(u32)        ((uint16_t) ((u32) >> 16))

/* (1234) -> 3412 */
#define mSWAP_BYTE(u16)         ((mLOW_MASK(u16) << 8) | mHIGH_BYTE(u16))
 
BOOLEAN bGet_descriptor_patch(void)
{
	if (mDEV_REQ_VALUE_HIGH() == 1)
	{
		uint8_t *p = (uint8_t *)u8UsbDeviceDescriptor;
		uint32_t u32Tmp=0;
		/* Copy Usb Device Descriptor */
		ath_hal_memcpy(UsbDeviceDescriptorPatch, p, sizeof(UsbDeviceDescriptorPatch));

		UsbDeviceDescriptorPatch[BCD_DEVICE_OFFSET] =
			BCD_DEVICE_FW_SIGNATURE;

		/* Patch for custom id from flash */
		if (bEepromExist == FALSE) {
			A_SFLASH_READ_4B(u32Tmp, FLASH_SIZE -
				EE_DATA_RESERVED_LEN + FLASH_USB_VENDOR_ID_OFFSET*2);
			UsbDeviceDescriptorPatch[VENDOR_ID_OFFSET] =
				mSWAP_BYTE(mLOW_WORD0(u32Tmp));
			UsbDeviceDescriptorPatch[PRODUCT_ID_OFFSET] =
				mSWAP_BYTE(mHIGH_WORD0(u32Tmp));
		}
      
		pu8DescriptorEX = UsbDeviceDescriptorPatch;
		u16TxRxCounter = mTABLE_LEN(u8UsbDeviceDescriptor[0]);
  
		if (u16TxRxCounter > mDEV_REQ_LENGTH())  
			u16TxRxCounter = mDEV_REQ_LENGTH();
             
		A_USB_EP0_TX_DATA();
           
		//u16TxRxCounter = 18;
		return TRUE;
	}  
	if (mDEV_REQ_VALUE_HIGH() == 2) {
		uint8_t *p = (uint8_t *)u8ConfigDescriptorEX;

		/* Copy ConfigDescriptor */
		ath_hal_memcpy(ConfigDescriptorPatch, p, sizeof(ConfigDescriptorPatch));

		 /* place holder for EPx patches */

		switch (mDEV_REQ_VALUE_LOW())
		{
		case 0x00:      // configuration no: 0
			pu8DescriptorEX = ConfigDescriptorPatch;
			u16TxRxCounter = ConfigDescriptorPatch[1];
			//u16TxRxCounter = 46;
			break;
		default:
			return FALSE;
		}

		if (u16TxRxCounter > mDEV_REQ_LENGTH())
			u16TxRxCounter = mDEV_REQ_LENGTH();

		A_USB_EP0_TX_DATA();
		return TRUE;
	}
	else {
		return bGet_descriptor();
	}
}

extern BOOLEAN bStandardCommand(void);

BOOLEAN bStandardCommand_patch(void)
{
	if (mDEV_REQ_REQ() == USB_SET_CONFIGURATION) {
		A_USB_SET_CONFIG();

#if ENABLE_SWAP_DATA_MODE
		// SWAP FUNCTION should be enabled while DMA engine is not working,
		// the best place to enable it is before we trigger the DMA
		MAGPIE_REG_USB_RX0_SWAP_DATA = 0x1;
		MAGPIE_REG_USB_TX0_SWAP_DATA = 0x1;

#if SYSTEM_MODULE_HP_EP5
		MAGPIE_REG_USB_RX1_SWAP_DATA = 0x1;
#endif

#if SYSTEM_MODULE_HP_EP6
		MAGPIE_REG_USB_RX2_SWAP_DATA = 0x1;
#endif

#endif //ENABLE_SWAP_DATA_MODE
		return TRUE;
	}
	else {
		return bStandardCommand();
	}
}
