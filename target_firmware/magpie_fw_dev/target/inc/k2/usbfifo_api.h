/*
 * @File: HIF_api.h
 * 
 * @Abstract: Host Interface api
 * 
 * @Notes:
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef _USB_FIFO_API_H
#define _USB_FIFO_API_H

#include "vbuf_api.h"

typedef struct _USB_FIFO_CONFIG {
        /* callback to get the buf for receiving commands from USB FIFO */
    VBUF* (*get_command_buf)(void);
        /* callback when receiving a command */
    void (*recv_command)(VBUF *cmd);    
        /* callback to get the buf for event to send to the host */
    VBUF* (*get_event_buf)(void);
        /* callback to indicate the event has been sent to the host */
    void (*send_event_done)(VBUF *buf);
    
        /* context used for all callbacks */
    //void *context;
} USB_FIFO_CONFIG;

/* hardware API table structure (API descriptions below) */
struct usbfifo_api {
    void (*_init)(USB_FIFO_CONFIG *pConfig);
    void (*_enable_event_isr)(void);

        /* room to expand this table by another table */
    void *pReserved;    
};

extern void usbfifo_module_install(struct usbfifo_api *apis);

#endif /* #ifndef _USB_FIFO_API_H */
