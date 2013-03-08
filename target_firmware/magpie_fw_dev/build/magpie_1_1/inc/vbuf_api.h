/*
 * @File: VBUF_api.h
 * 
 * @Abstract: Host Interface api
 * 
 * @Notes:
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef _VBUF_API_H
#define _VBUF_API_H

#include <vdesc_api.h>

#define MAX_BUF_CTX_LEN     20

typedef struct _VBUF
{
    VDESC           *desc_list;
    struct _VBUF    *next_buf; 
    A_UINT16        buf_length; 
    A_UINT8         reserved[2];
    A_UINT8         ctx[MAX_BUF_CTX_LEN];  
    //A_UINT8         end_point;    
    //A_UINT8         reserved[1]; 
} VBUF;

#define VBUF_GET_DATA_ADDR(vbuf)    (vbuf->desc_list->buf_addr + vbuf->desc_list->data_offset)

/* hardware API table structure (API descriptions below) */
struct vbuf_api {
    void (*_init)(int nBuf);
    VBUF* (*_alloc_vbuf)(void);
    VBUF* (*_alloc_vbuf_with_size)(int size, int reserve);
    void (*_free_vbuf)(VBUF *buf);

        /* room to expand this table by another table */
    void *pReserved;    
};

extern void vbuf_module_install(struct vbuf_api *apis);

#endif /* #ifndef _HIF_API_H */
