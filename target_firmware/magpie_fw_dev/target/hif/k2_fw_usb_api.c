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

typedef void (* USBFIFO_recv_command)(VBUF *cmd);
void _fw_usb_suspend_reboot();

extern Action      eUsbCxFinishAction;
extern CommandType eUsbCxCommand;
extern BOOLEAN     UsbChirpFinish;
extern USB_FIFO_CONFIG usbFifoConf;

USBFIFO_recv_command m_origUsbfifoRecvCmd = NULL;

#if SYSTEM_MODULE_USB
#define vUsb_ep0end(void)                                   \
{                                                           \
    eUsbCxCommand = CMD_VOID;                               \
    USB_BYTE_REG_WRITE(ZM_CX_CONFIG_STATUS_OFFSET, 0x01);   \
}

#define vUsb_ep0fail(void)  USB_BYTE_REG_WRITE(ZM_CX_CONFIG_STATUS_OFFSET, 0x04)

#define vUsb_rst()                                              \
{                                                               \
    USB_BYTE_REG_WRITE(ZM_INTR_SOURCE_7_OFFSET,                 \
        (USB_BYTE_REG_READ(ZM_INTR_SOURCE_7_OFFSET)&~BIT1));    \
    UsbChirpFinish = FALSE;                                     \
}

#define vUsb_suspend()  USB_BYTE_REG_WRITE(ZM_INTR_SOURCE_7_OFFSET, \
                            (USB_BYTE_REG_READ(ZM_INTR_SOURCE_7_OFFSET)&~BIT2))

#define vUsb_resm() USB_BYTE_REG_WRITE(ZM_INTR_SOURCE_7_OFFSET,     \
                        (USB_BYTE_REG_READ(ZM_INTR_SOURCE_7_OFFSET)&~BIT3))

void _fw_usbfifo_recv_command(VBUF *buf)
{
    A_UINT8 *cmd_data;
    A_UINT32 tmp;

    cmd_data = (A_UINT8 *)(buf->desc_list->buf_addr + buf->desc_list->data_offset);
    tmp = *((A_UINT32 *)cmd_data);
    if ( tmp == 0xFFFFFFFF ) {
        _fw_usb_suspend_reboot();
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

#define CHECK_SOF_LOOP_CNT    50

void _fw_usb_suspend_reboot()
{
    volatile uint32_t gpio_in = 0;
    volatile uint32_t pupd = 0;
    volatile uint32_t t = 0;
    volatile uint32_t sof_no=0,sof_no_new=0;
    /* Set GO_TO_SUSPEND bit to USB main control register */
    vUsb_suspend();
    A_PRINTF("!USB suspend\n\r");

    // keep the record of suspend
#if defined(PROJECT_MAGPIE)
    *((volatile uint32_t*)WATCH_DOG_MAGIC_PATTERN_ADDR) = SUS_MAGIC_PATTERN;
#elif defined(PROJECT_K2)
    HAL_WORD_REG_WRITE(MAGPIE_REG_RST_STATUS_ADDR, SUS_MAGIC_PATTERN);
#endif /* #if defined(PROJECT_MAGPIE) */

    /* Reset USB FIFO */
    A_USB_RESET_FIFO();

    /* Turn off power */
    A_USB_POWER_OFF();

    DEBUG_SYSTEM_STATE = (DEBUG_SYSTEM_STATE&(~0xffff)) | 0x1000;

    // reset ep3/ep4 fifo in case there is data which might affect resuming
//  HAL_BYTE_REG_WRITE(0x100ae, (HAL_BYTE_REG_READ(0x100ae)|0x10));
//  HAL_BYTE_REG_WRITE(0x100ae, (HAL_BYTE_REG_READ(0x100af)|0x10));

    {
        // config gpio to input before goto suspend

        //disable JTAG/ICE
        //jtag = HAL_WORD_REG_READ(0x10004054);
        //HAL_WORD_REG_WRITE(0x10004054, (jtag|BIT17));
                
		//disable SPI
        //spi = HAL_WORD_REG_READ(0x50040);
        //HAL_WORD_REG_WRITE(0x50040, (spi&~(BIT8)));
                
		//set all GPIO to input
        gpio_in = HAL_WORD_REG_READ(0x1000404c);
        HAL_WORD_REG_WRITE(0x1000404c, 0x0);
                
		//set PU/PD for all GPIO except two UART pins
        pupd = HAL_WORD_REG_READ(0x10004088);
        HAL_WORD_REG_WRITE(0x10004088, 0xA982AA6A);
    }

    sof_no= HAL_WORD_REG_READ(0x10004); 
    for (t = 0; t < CHECK_SOF_LOOP_CNT; t++)
    {
        A_DELAY_USECS(1000);    //delay 1ms	
        sof_no_new = HAL_WORD_REG_READ(0x10004);

        if(sof_no_new == sof_no)
     	    break; 

        sof_no = sof_no_new;	  
    } 
    
    /* 
     * Reset "printf" module patch point(RAM to ROM) when K2 warm start or suspend,  
     * which fixed the error issue cause by redownload another different firmware. 
     */
    _indir_tbl.cmnos.printf._printf = save_cmnos_printf;
    
    ///////////////////////////////////////////////////////////////
    // setting the go suspend here, power down right away...
    if (t != CHECK_SOF_LOOP_CNT)   // not time out
        HAL_WORD_REG_WRITE(0x10000, HAL_WORD_REG_READ(0x10000)|(0x8));
    ///////////////////////////////////////////////////////////////

    DEBUG_SYSTEM_STATE = (DEBUG_SYSTEM_STATE&(~0xffff)) | 0x1100;

#if 0 // pll unstable, h/w bug?
    HAL_WORD_REG_WRITE(0x50040, (0x300|6|(1>>1)<<12));
    A_UART_HWINIT((40*1000*1000)/1, 19200);
#endif
    {
        // restore gpio setting
        //HAL_WORD_REG_WRITE(0x10004054, jtag);
        //HAL_WORD_REG_WRITE(0x50040, spi);
        HAL_WORD_REG_WRITE(0x1000404c, gpio_in);
        HAL_WORD_REG_WRITE(0x10004088, pupd);
    }
    DEBUG_SYSTEM_STATE = (DEBUG_SYSTEM_STATE&(~0xffff)) | 0x1200;

    {
        // since we still need to touch mac_base address after resuming back, so that
        // reset mac can't be done in ResetFifo function, move to here... 
        // whole mac control reset.... (bit1)
        HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, (BIT1) );
        HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, (HAL_WORD_REG_READ(MAGPIE_REG_RST_PWDN_CTRL_ADDR)|BIT0));
        HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, 0x0 );
	A_DELAY_USECS(1000);
    }

    //A_PRINTF("reg(0x10020)=(%x)\n", HAL_WORD_REG_READ(0x10020));
    // disable ep3 int enable, so that resume back won't send wdt magic pattern out!!!
    mUSB_STATUS_IN_INT_DISABLE();            

    MAGPIE_REG_USB_RX0_SWAP_DATA = 0x1;
    MAGPIE_REG_USB_TX0_SWAP_DATA = 0x1;
    MAGPIE_REG_USB_RX1_SWAP_DATA = 0x1;
    MAGPIE_REG_USB_RX2_SWAP_DATA = 0x1;

    if (((DEBUG_SYSTEM_STATE&~(0x0000ffff))>>16 == 0x5342)) {
        /* UART_SEL and SPI_SEL */
        HAL_WORD_REG_WRITE(0x50040, (0x300|0|(1>>1)<<12));
    }

    /* Jump to boot code */
    A_USB_JUMP_BOOT();

}

/*
 * -- patch usb_fw_task --
 * . usb zero length interrupt should not clear by s/w, h/w will handle that
 * . complete suspend handle, configure gpio, turn off related function, 
 *   slow down the pll for stable issue
 */
void _fw_usb_fw_task(void)
{
    register uint8_t usb_interrupt_level1;
    register uint8_t usb_interrupt_level2;

    usb_interrupt_level1 = USB_BYTE_REG_READ(ZM_INTR_GROUP_OFFSET);
#if 0 // these endpoints are handled by DMA
    if (usb_interrupt_level1 & BIT5)            //Group Byte 5
    {
        vUsb_Data_In();
    }
#endif
    if (usb_interrupt_level1 & BIT4)
    {
        usb_interrupt_level2 = USB_BYTE_REG_READ(ZM_INTR_SOURCE_4_OFFSET);
        if( usb_interrupt_level2 & BIT6)
            A_USB_REG_OUT();//vUsb_Reg_Out();
    }

    if (usb_interrupt_level1 & BIT6)
    {
        //zfGenWatchDogEvent();
    usb_interrupt_level2 = USB_BYTE_REG_READ(ZM_INTR_SOURCE_6_OFFSET);
        if( usb_interrupt_level2 & BIT6)
             A_USB_STATUS_IN();//vUsb_Status_In();
    }

    if (usb_interrupt_level1 & BIT0)            //Group Byte 0
    {
        //usb_interrupt_level2 = ZM_INTR_SOURCE_0_REG;
        usb_interrupt_level2 = USB_BYTE_REG_READ(ZM_INTR_SOURCE_0_OFFSET);

        // refer to FUSB200, p 48, offset:21H, bit7 description, should clear the command abort interrupt first!?
        if (usb_interrupt_level2 & BIT7)
        {
            //ZM_INTR_SOURCE_0_REG &= 0x7f;       // Handle command abort
            USB_BYTE_REG_WRITE(ZM_INTR_SOURCE_0_OFFSET, (USB_BYTE_REG_READ(ZM_INTR_SOURCE_0_OFFSET)& ~BIT7));
            A_PRINTF("![SOURCE_0] bit7 on\n\r");
        }

        if (usb_interrupt_level2 & BIT1)
        {
            //A_PRINTF("![USB] ep0 IN in \n\r");
            A_USB_EP0_TX();                       // USB EP0 tx interrupt
        }
        if (usb_interrupt_level2 & BIT2)
        {
            //A_PRINTF("![USB] ep0 OUT in\n\r");
            A_USB_EP0_RX();                       // USB EP0 rx interrupt
        }
        if (usb_interrupt_level2 & BIT0)
        {
            //A_PRINTF("![USB] ep0 SETUP in\n\r");
            A_USB_EP0_SETUP();
            //vWriteUSBFakeData();
        }
//        else if (usb_interrupt_level2 & BIT3)
        if (usb_interrupt_level2 & BIT3)
        {
            vUsb_ep0end();
//            A_PRINTF("![SOURCE_0] ep0 CMD_END\n\r");
        }
        if (usb_interrupt_level2 & BIT4)
        {
            vUsb_ep0fail();
//            A_PRINTF("![SOURCE_0] ep0 CMD_FAIL\n\r");
        }
        if (eUsbCxFinishAction == ACT_STALL)
        {
            // set CX_STL to stall Endpoint0 & will also clear FIFO0
            USB_BYTE_REG_WRITE(ZM_CX_CONFIG_STATUS_OFFSET, 0x04);
//            A_PRINTF("![USB] ZM_CX_CONFIG_STATUS_REG = 0x04\n\r");
        }
        else if (eUsbCxFinishAction == ACT_DONE)
        {
            // set CX_DONE to indicate the transmistion of control frame
            USB_BYTE_REG_WRITE(ZM_CX_CONFIG_STATUS_OFFSET, 0x01);
        }
        eUsbCxFinishAction = ACT_IDLE;
    }

    if (usb_interrupt_level1 & BIT7)            //Group Byte 7
    {
        //usb_interrupt_level2 = ZM_INTR_SOURCE_7_REG;
        usb_interrupt_level2 = USB_BYTE_REG_READ(ZM_INTR_SOURCE_7_OFFSET);

#if 0
        if (usb_interrupt_level2 & BIT7)
        {
            vUsb_Data_Out0Byte();
//            A_PRINTF("![SOURCE_7] bit7 on, clear it\n\r");
        }
        if (usb_interrupt_level2 & BIT6)
        {
            vUsb_Data_In0Byte();
//            A_PRINTF("![SOURCE_7] bit6 on, clear it\n\r");
        }
#endif
        
        if (usb_interrupt_level2 & BIT1)
        {
            vUsb_rst();
            //USB_BYTE_REG_WRITE(ZM_INTR_SOURCE_7_REG, (USB_BYTE_REG_READ(ZM_INTR_SOURCE_7_OFFSET)&~0x2));
            A_PRINTF("!USB reset\n\r");
//            A_PRINTF("![0x1012c]: %\n\r", USB_WORD_REG_READ(0x12c));
        }
        if (usb_interrupt_level2 & BIT2)
        {
           // TBD: the suspend resume code should put here, Ryan, 07/18
           //
           //  issue, jump back to rom code and what peripherals should we reset here?
           //
           _fw_usb_suspend_reboot();		
        }
        if (usb_interrupt_level2 & BIT3)
        {
            vUsb_resm();
            A_PRINTF("!USB resume\n\r");
        }
    }

}


void _fw_usb_reset_fifo(void)
{
    volatile uint32_t   *reg_data;

    HAL_BYTE_REG_WRITE(0x100ae, (HAL_BYTE_REG_READ(0x100ae)|0x10));
    HAL_BYTE_REG_WRITE(0x100af, (HAL_BYTE_REG_READ(0x100af)|0x10));

    // disable ep3 int enable, so that resume back won't send wdt magic pattern out!!!
    mUSB_STATUS_IN_INT_DISABLE();

    // update magic pattern to indicate this is a suspend
    // k2: MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR
    // magpie: MAGPIE_REG_RST_STATUS_ADDR
    HAL_WORD_REG_WRITE(MAGPIE_REG_RST_STATUS_ADDR, SUS_MAGIC_PATTERN);

    /*
     * Before USB suspend, USB DMA must be reset(refer to Otus)
     * Otus runs the following statements only
     * HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, BIT0|BIT2 );
     * HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, 0x0 );
     * K2 must run the following statements additionally
     * reg_data = (A_UINT32 *)(USB_CTRL_BASE_ADDRESS + 0x118);
     * *reg_data = 0x00000000;
     * *reg_data = 0x00000001;
     * because of Hardware bug in K2
     */
    reg_data = (uint32_t *)(USB_CTRL_BASE_ADDRESS + 0x118);
    *reg_data = 0x00000000;

    // reset both usb(bit2)/wlan(bit1) dma
    HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, (BIT2) );
    HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, (HAL_WORD_REG_READ(MAGPIE_REG_RST_PWDN_CTRL_ADDR)|BIT0));
    HAL_WORD_REG_WRITE( MAGPIE_REG_RST_PWDN_CTRL_ADDR, 0x0 );

    *reg_data = 0x00000001;

    /* MAC warem reset */
    //reg_data = (uint32_t *)(K2_REG_MAC_BASE_ADDR + 0x7000);
    //*reg_data = 0x00000001;

    //A_DELAY_USECS(1);

    //*reg_data = 0x00000000;

    //while (*reg_data)   ;

    A_PRINTF("\n change clock to 22 and go to suspend now!");
    
    /* UART_SEL */
    HAL_WORD_REG_WRITE(0x50040, (0x200|0|(1>>1)<<12));
    A_UART_HWINIT((22*1000*1000), 19200);
}

void cold_reboot(void)
{
	A_PRINTF("Cold reboot initiated.");
#if defined(PROJECT_MAGPIE)
	HAL_WORD_REG_WRITE(WATCH_DOG_MAGIC_PATTERN_ADDR, 0);
#elif defined(PROJECT_K2)
	HAL_WORD_REG_WRITE(MAGPIE_REG_RST_STATUS_ADDR, 0);
#endif /* #if defined(PROJECT_MAGPIE) */
	A_USB_JUMP_BOOT();
}

/*
 * -- support more than 64 bytes command on ep4 -- 
 */
void vUsb_Reg_Out_patch(void)
{   
    uint16_t usbfifolen;
    uint16_t ii;
    uint32_t ep4_data;
    static volatile uint32_t *regaddr;    // = (volatile uint32_t *) ZM_CMD_BUFFER;
    static uint16_t cmdLen;
    static VBUF *buf;
    BOOLEAN cmd_is_last = FALSE;
    static BOOLEAN cmd_is_new = TRUE;

    // get the size of this transcation
    usbfifolen = USB_BYTE_REG_READ(ZM_EP4_BYTE_COUNT_LOW_OFFSET);
    if (usbfifolen > 0x40) {
        A_PRINTF("EP4 FIFO Bug? Buffer is too big: %x\n", usbfifolen);
        cold_reboot();
    }

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
    //
    // zero-length supposed should be set through 0x27/bit7->0x19/bit4, not here
    //
    if( usbfifolen<bUSB_EP_MAX_PKT_SIZE_64 ) {
        cmd_is_last = TRUE;
    }
    
    // accumulate the size
    cmdLen += usbfifolen;
    if (cmdLen > buf->desc_list->buf_size) {
        A_PRINTF("Data length on EP4 FIFO is bigger as allocated buffer data!"
                 " Drop it!\n");
        goto ERR;
    }
    
    // round it to alignment
    if(usbfifolen % 4)
        usbfifolen = (usbfifolen >> 2) + 1;
    else
        usbfifolen = usbfifolen >> 2;
    
//    A_PRINTF("copy data out from fifo to - %p\n\r", regaddr);
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
    //A_PRINTF("bSet_configuration...\n\r");

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
    
    }

//    if( mBufLen>bUSB_EP_MAX_PKT_SIZE_64 )
//        A_PRINTF("EP3 send %d bytes to host \n", mBufLen);

//    while(1)
    {
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
        
//        if( mBufLen<=bUSB_EP_MAX_PKT_SIZE_64 )
//            break;   
    }

    if ( evntbuf != NULL && cmdEnd )
    {
        usbFifoConf.send_event_done(evntbuf);
        cmd_is_new = TRUE;
    }
    
ERR_DONE:
    ;
}

extern uint16_t *u8UsbDeviceDescriptor;
extern uint16_t *u8ConfigDescriptorEX;
extern uint16_t *pu8DescriptorEX;
extern uint16_t u16TxRxCounter;
extern BOOLEAN bGet_descriptor(void);

uint16_t DeviceDescriptorPatch[9];
uint16_t ConfigDescriptorPatch[30];


#define BCD_DEVICE		    6
#define BCD_DEVICE_FW_SIGNATURE	    0xffff
#define EP3_TRANSFER_TYPE_OFFSET    17
#define EP3_INT_INTERVAL            19
#define EP4_TRANSFER_TYPE_OFFSET    21
#define EP4_INT_INTERVAL            22

BOOLEAN bGet_descriptor_patch(void)
{
    int i;
    switch (mDEV_REQ_VALUE_HIGH()) {
    case 1:
        ath_hal_memcpy(DeviceDescriptorPatch,
            u8UsbDeviceDescriptor, sizeof(DeviceDescriptorPatch));

        DeviceDescriptorPatch[BCD_DEVICE] = BCD_DEVICE_FW_SIGNATURE;

        pu8DescriptorEX = DeviceDescriptorPatch;
        u16TxRxCounter = mTABLE_LEN(DeviceDescriptorPatch[0]);
        break;
    case 2:
        /* Copy ConfigDescriptor */
	ath_hal_memcpy(ConfigDescriptorPatch,
            u8ConfigDescriptorEX, sizeof(ConfigDescriptorPatch));

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
        break;
    default:
        return bGet_descriptor();
    }

    if (u16TxRxCounter > mDEV_REQ_LENGTH())
        u16TxRxCounter = mDEV_REQ_LENGTH();

    A_USB_EP0_TX_DATA();
    return TRUE;
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

#endif


