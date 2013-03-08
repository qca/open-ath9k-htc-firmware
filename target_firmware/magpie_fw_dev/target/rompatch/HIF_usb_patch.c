#include "sys_cfg.h"
#include "dt_defs.h"
#include "reg_defs.h"

#include <osapi.h>
#include <hif_api.h>
#include <Magpie_api.h>
#include <vdesc_api.h>
#include <adf_os_mem.h> 
#include <adf_os_io.h>

#include "hif_usb.h"

/*
 * -- support more than 64 bytes command on ep4 -- 
 */
int _HIFusb_get_max_msg_len_patch(hif_handle_t handle, int pipe)
{
    switch(pipe) {
        case HIF_USB_PIPE_INTERRUPT:
        case HIF_USB_PIPE_COMMAND:
            return 512;
            
        default:
            return 1600;
    }
}

/*
 * -- move the usb_task to here --
 */
void _HIFusb_isr_handler_patch(hif_handle_t h)
{
    A_USB_FW_TASK();

    _HIFusb_isr_handler();
}


/*
 * -- reset usb dma --
 *
 * - make sure DMA_START bit0 is zero
 * - update DMA_START bit4 to 1
 * - update DESC_START_ADDR
 * - update DMA_START bit 0
 */
void _HIFusb_start_patch(hif_handle_t handle) 
{
    MAGPIE_REG_USB_TX0_DMA_START = 0x0;
    MAGPIE_REG_USB_RX0_DMA_START = 0x0;
    MAGPIE_REG_USB_RX1_DMA_START = 0x0;
    MAGPIE_REG_USB_RX2_DMA_START = 0x0;
    
    while( 1 )
    {
        if(!MAGPIE_REG_USB_TX0_DMA_START &&
	   !MAGPIE_REG_USB_RX0_DMA_START &&
	   !MAGPIE_REG_USB_RX1_DMA_START &&
	   !MAGPIE_REG_USB_RX2_DMA_START )
        {
            MAGPIE_REG_USB_TX0_DMA_START = MAGPIE_REG_USB_TX0_DMA_START|BIT4; 
            MAGPIE_REG_USB_RX0_DMA_START = MAGPIE_REG_USB_RX0_DMA_START|BIT4;
            MAGPIE_REG_USB_RX1_DMA_START = MAGPIE_REG_USB_RX1_DMA_START|BIT4;
            MAGPIE_REG_USB_RX2_DMA_START = MAGPIE_REG_USB_RX2_DMA_START|BIT4;
            break;
        }
    }
    _HIFusb_start();
}
