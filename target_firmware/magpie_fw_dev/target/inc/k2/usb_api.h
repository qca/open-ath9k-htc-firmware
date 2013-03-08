#ifndef USB_API_H
#define USB_API_H

#include "dt_defs.h"

/******** hardware API table structure (API descriptions below) *************/
struct usb_api {
    void (*_usb_init)(void);
    void (*_usb_rom_task)(void);
    void (*_usb_fw_task)(void);
    void (*_usb_init_phy)(void);

    // ep0 operation
    void (*_usb_ep0_setup)(void);
    
    void (*_usb_ep0_tx)(void);
    void (*_usb_ep0_rx)(void);

    // get/set interface
    BOOLEAN (*_usb_get_interface)(void);
    BOOLEAN (*_usb_set_interface)(void);

    // get/set configuration
    BOOLEAN (*_usb_get_configuration)(void);
    BOOLEAN (*_usb_set_configuration)(void);

    // standard/vendor command
    BOOLEAN (*_usb_standard_cmd)(void);    
    void (*_usb_vendor_cmd)(void);

    void (*_usb_power_off)(void);
    void (*_usb_reset_fifo)(void);
    void (*_usb_gen_wdt)(void);
    void (*_usb_jump_boot)(void);
    
    BOOLEAN (*_usb_clr_feature)(void);
    BOOLEAN (*_usb_set_feature)(void);    
    BOOLEAN (*_usb_set_address)(void);
    BOOLEAN (*_usb_get_descriptor)(void);

    BOOLEAN (*_usb_get_status)(void);
    void (*_usb_setup_desc)(void);
    void (*_usb_reg_out)(void);
    void (*_usb_status_in)(void);

    void (*_usb_ep0_tx_data)(void);
    void (*_usb_ep0_rx_data)(void);

    void (*_usb_clk_init)(void);
};

#endif
