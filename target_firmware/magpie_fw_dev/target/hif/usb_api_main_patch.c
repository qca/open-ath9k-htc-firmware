
#include "usb_defs.h"
#include "usb_type.h"
#include "usb_pre.h"
#include "usb_extr.h"
#include "usb_std.h"
#include "reg_defs.h"
#include "athos_api.h"
#include "usbfifo_api.h"

#include "sys_cfg.h"

#define USB_EP4_MAX_PKT_SIZE bUSB_EP_MAX_PKT_SIZE_64
#define USB_EP3_MAX_PKT_SIZE bUSB_EP_MAX_PKT_SIZE_64

extern USB_FIFO_CONFIG usbFifoConf;

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
 * support more than 64 bytes command on ep4 
 */
void usb_reg_out_patch(void)
{
	uint16_t usbfifolen;
	uint16_t ii;
	uint32_t ep4_data;
	static volatile uint32_t *regaddr;
	static uint16_t cmd_len;
	static VBUF *buf;
	BOOLEAN cmd_is_last = FALSE;
	static BOOLEAN cmd_is_new = TRUE;

	/* get the size of this transcation */
	usbfifolen = USB_BYTE_REG_READ(ZM_EP4_BYTE_COUNT_LOW_OFFSET);

	if (usbfifolen > USB_EP4_MAX_PKT_SIZE) {
		A_PRINTF("EP4 FIFO Bug? Buffer is too big: %x\n", usbfifolen);
		cold_reboot();
	}

	/* check is command is new */
	if(cmd_is_new) {

		buf = usbFifoConf.get_command_buf();
		cmd_len = 0;

		if(!buf) {
			A_PRINTF("%s: Filed to get new buffer.\n", __func__);
			goto err;
		}

		/* copy free, assignment buffer of the address */
		regaddr = (uint32_t *)buf->desc_list->buf_addr;

		cmd_is_new = FALSE;
	}

	/* just in case, suppose should not happen */
	if(!buf)
		goto err;

	/* if size is smaller, this is the last command!
	 * zero-length supposed should be set through 0x27/bit7->0x19/bit4, not here
	 */
	if(usbfifolen < USB_EP4_MAX_PKT_SIZE)
		cmd_is_last = TRUE;

	/* accumulate the size */
	cmd_len += usbfifolen;

	if (cmd_len > buf->desc_list->buf_size) {
		A_PRINTF("%s: Data length on EP4 FIFO is bigger as "
			 "allocated buffer data! Drop it!\n", __func__);
		goto err;
	}

	/* round it to alignment */
	if(usbfifolen % 4)
		usbfifolen = (usbfifolen >> 2) + 1;
	else
		usbfifolen = usbfifolen >> 2;

	/* retrieve the data from fifo */
	for(ii = 0; ii < usbfifolen; ii++) {
		/* read fifo data out */
		ep4_data = USB_WORD_REG_READ(ZM_EP4_DATA_OFFSET);
		*regaddr = ep4_data;
		regaddr++;
	}

	/* if this is the last command, callback to HTC */
	if (cmd_is_last) {
		buf->desc_list->next_desc = NULL;
		buf->desc_list->data_offset = 0;
		buf->desc_list->data_size = cmd_len;
		buf->desc_list->control = 0;
		buf->next_buf = NULL;
		buf->buf_length = cmd_len;

		usbFifoConf.recv_command(buf);

		cmd_is_new = TRUE;
	}

	goto done;
err:
	/* we might get no command buffer here?
	 * but if we return here, the ep4 fifo will be lock out,
	 * so that we still read them out but just drop it? */
	for(ii = 0; ii < usbfifolen; ii++)
		ep4_data = USB_WORD_REG_READ(ZM_EP4_DATA_OFFSET);

done:
	/* mUSB_STATUS_IN_INT_ENABLE(); */
	;
}

