/*
 * @File: 
 * 
 * @Abstract: Buf pool implementation: static version
 * 
 * @Notes: 
 * 
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#include <osapi.h>
#include <Magpie_api.h> 
#include <cmnos_api.h>
#include <buf_pool_api.h>
#include <vbuf_api.h>
#include <vdesc_api.h>
#include <adf_os_mem.h> 

#include "buf_pool_static.h"

LOCAL htc_handle_t _buf_pool_static_init(adf_net_handle_t handle);
LOCAL void _buf_pool_static_create_pool(pool_handle_t handle, BUF_POOL_ID poolId, int nItems, int nSize);
LOCAL adf_nbuf_t  _buf_pool_static_alloc_buf(pool_handle_t handle, BUF_POOL_ID poolId, int reserve);
LOCAL adf_nbuf_t  _buf_pool_static_alloc_buf_align(pool_handle_t handle, BUF_POOL_ID poolId, int reserve, int align);
LOCAL void _buf_pool_static_free_buf(pool_handle_t handle, BUF_POOL_ID poolId, adf_nbuf_t buf);
LOCAL void _buf_pool_static_shutdown(pool_handle_t handle);      

BUF_POOL_STATIC_CONTEXT g_poolCtx;

void buf_pool_module_install(struct buf_pool_api *pAPIs)
{   
    pAPIs->_init = _buf_pool_static_init;
    pAPIs->_create_pool = _buf_pool_static_create_pool;
    pAPIs->_alloc_buf = _buf_pool_static_alloc_buf;
    pAPIs->_alloc_buf_align = _buf_pool_static_alloc_buf_align;
    pAPIs->_free_buf = _buf_pool_static_free_buf;
    pAPIs->_shutdown = _buf_pool_static_shutdown;
}
 
LOCAL pool_handle_t _buf_pool_static_init(adf_os_handle_t handle)
{
#if 1
    int i;
    
    for(i=0; i < POOL_ID_MAX; i++) {
        g_poolCtx.bufQ[i] = NULL;
    }
    
    return &g_poolCtx;
#else    
    BUF_POOL_STATIC_CONTEXT *ctx;
    
    //ctx = (BUF_POOL_static_CONTEXT *)A_ALLOCRAM(sizeof(BUF_POOL_static_CONTEXT));
    ctx = (BUF_POOL_STATIC_CONTEXT *)adf_os_mem_alloc(sizeof(BUF_POOL_STATIC_CONTEXT));
    ctx->NetHandle = handle;
    
    return ctx; 
#endif    
}      
    
LOCAL void _buf_pool_static_shutdown(pool_handle_t handle) 
{
    // SHALL NOT BE USED in FW
}

LOCAL void _buf_pool_static_create_pool(pool_handle_t handle, BUF_POOL_ID poolId, int nItems, int nSize)
{
    int i;
    VBUF *buf;
    VDESC *desc;
    
    //BUF_POOL_STATIC_CONTEXT *ctx = (BUF_POOL_STATIC_CONTEXT *)handle;
    
    for ( i = 0; i < nItems; i++) {
        buf = VBUF_alloc_vbuf();
        desc = VDESC_alloc_vdesc();

        desc->buf_addr = (A_UINT8 *)adf_os_mem_alloc(nSize);
        desc->buf_size = nSize;
        desc->data_offset = 0;
        desc->data_size = 0;
        
        buf->buf_length = 0;        
        buf->desc_list = desc;
        
        if ( g_poolCtx.bufQ[poolId] == NULL ) {
            g_poolCtx.bufQ[poolId] = buf;
        } else {
            buf->next_buf = g_poolCtx.bufQ[poolId];
            g_poolCtx.bufQ[poolId] = buf;
        }
    }
}
            
LOCAL adf_nbuf_t  _buf_pool_static_alloc_buf(pool_handle_t handle, BUF_POOL_ID poolId, int reserve)
{
    VBUF *buf;
    
    buf = g_poolCtx.bufQ[poolId];
    if ( buf != NULL ) {
        g_poolCtx.bufQ[poolId] = buf->next_buf;
        
        buf->next_buf = NULL;
        buf->desc_list->data_offset = reserve;
        buf->desc_list->data_size = 0;
        buf->buf_length = 0;
    }
    
    return buf;
}

LOCAL adf_nbuf_t  _buf_pool_static_alloc_buf_align(pool_handle_t handle, BUF_POOL_ID poolId, int reserve, int align)
{
    return _buf_pool_static_alloc_buf(handle, poolId, reserve);
}
    
LOCAL void _buf_pool_static_free_buf(pool_handle_t handle, BUF_POOL_ID poolId, adf_nbuf_t buf)
{
    if ( g_poolCtx.bufQ[poolId] == NULL ) {
        g_poolCtx.bufQ[poolId] = buf;
    } else {
        buf->next_buf = g_poolCtx.bufQ[poolId];
        g_poolCtx.bufQ[poolId] = buf;
    }
}
