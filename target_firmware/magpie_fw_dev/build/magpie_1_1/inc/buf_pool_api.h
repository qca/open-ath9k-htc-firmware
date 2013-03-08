/*
 * @File: buf_pool_api.h
 * 
 * @Abstract: BUF Pool api
 * 
 * @Notes:
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef _BUF_POOL_API_H
#define _BUF_POOL_API_H

#include <adf_nbuf.h>

/* endpoint defines */
typedef enum
{
    POOL_ID_HTC_CONTROL         = 0, 
    POOL_ID_WMI_SVC_CMD_REPLY   = 1,  
    POOL_ID_WMI_SVC_EVENT       = 2,
    POOL_ID_WLAN_RX_BUF         = 3,
    POOL_ID_MAX                 = 10 
} BUF_POOL_ID;

typedef void* pool_handle_t;

/* hardware API table structure (API descriptions below) */
struct buf_pool_api {
    pool_handle_t (*_init)(adf_os_handle_t handle);
            
    void (*_shutdown)(pool_handle_t handle);
    
    void (*_create_pool)(pool_handle_t handle, BUF_POOL_ID poolId, int nItems, int nSize);
    
    adf_nbuf_t  (*_alloc_buf)(pool_handle_t handle, BUF_POOL_ID poolId, int reserve);
    
    adf_nbuf_t  (*_alloc_buf_align)(pool_handle_t handle, BUF_POOL_ID poolId, int reserve, int align);
    
    void (*_free_buf)(pool_handle_t handle, BUF_POOL_ID poolId, adf_nbuf_t buf);
    
        /* room to expand this table by another table */
    void *pReserved;    
};

extern void buf_pool_module_install(struct buf_pool_api *apis);

#endif /* #ifndef _BUF_POOL_API_H */
