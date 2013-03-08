/*
 * @File: 
 * 
 * @Abstract: Buf pool implementation: Dynamic version
 * 
 * @Notes: 
 * 
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
 */
#include <adf_os_mem.h>
#include <adf_os_module.h>
#include <osapi.h>
#include <Magpie_api.h> 
//#include <os/cmnos_api.h>
#include <buf_pool_api.h>
 
LOCAL htc_handle_t _buf_pool_dynamic_init(adf_os_handle_t handle);
LOCAL void _buf_pool_dynamic_create_pool(pool_handle_t handle, BUF_POOL_ID poolId, int nItems, int nSize);
LOCAL adf_nbuf_t  _buf_pool_dynamic_alloc_buf(pool_handle_t handle, BUF_POOL_ID poolId, int reserve);
LOCAL adf_nbuf_t  _buf_pool_dynamic_alloc_buf_align(pool_handle_t handle, BUF_POOL_ID poolId, int reserve, int align);
LOCAL void _buf_pool_dynamic_free_buf(pool_handle_t handle, BUF_POOL_ID poolId, adf_nbuf_t buf);
LOCAL void _buf_pool_dynamic_shutdown(pool_handle_t handle);
       
typedef struct _POOL_CONFIG {
    int nSize;
} POOL_CONFIG;
       
typedef struct _BUF_POOL_DYNAMIC_CONTEXT {
    adf_os_handle_t  OSHandle;
    POOL_CONFIG poolConf[POOL_ID_MAX]; 
} BUF_POOL_DYNAMIC_CONTEXT;

void buf_pool_module_install(struct buf_pool_api *pAPIs)
{   
    pAPIs->_init = _buf_pool_dynamic_init;
    pAPIs->_create_pool = _buf_pool_dynamic_create_pool;
    pAPIs->_alloc_buf = _buf_pool_dynamic_alloc_buf;
    pAPIs->_alloc_buf_align = _buf_pool_dynamic_alloc_buf_align;
    pAPIs->_free_buf = _buf_pool_dynamic_free_buf;
    pAPIs->_shutdown = _buf_pool_dynamic_shutdown;
}
 
LOCAL pool_handle_t _buf_pool_dynamic_init(adf_os_handle_t handle)
{
    BUF_POOL_DYNAMIC_CONTEXT *ctx;
    
    ctx = (BUF_POOL_DYNAMIC_CONTEXT *)adf_os_mem_alloc(sizeof(BUF_POOL_DYNAMIC_CONTEXT));
    ctx->OSHandle = handle;
    
    return ctx; 
}      
    
LOCAL void _buf_pool_dynamic_shutdown(pool_handle_t handle) 
{
    BUF_POOL_DYNAMIC_CONTEXT *ctx = (BUF_POOL_DYNAMIC_CONTEXT *)handle;
    
    adf_os_mem_free(ctx);
}

LOCAL void _buf_pool_dynamic_create_pool(pool_handle_t handle, BUF_POOL_ID poolId, int nItems, int nSize)
{
    BUF_POOL_DYNAMIC_CONTEXT *ctx = (BUF_POOL_DYNAMIC_CONTEXT *)handle;
    
    ctx->poolConf[poolId].nSize = nSize;
}
            
LOCAL adf_nbuf_t  _buf_pool_dynamic_alloc_buf(pool_handle_t handle, BUF_POOL_ID poolId, int reserve)
{
    BUF_POOL_DYNAMIC_CONTEXT *ctx = (BUF_POOL_DYNAMIC_CONTEXT *)handle;
    POOL_CONFIG *poolConf = &ctx->poolConf[poolId];
            
    return adf_nbuf_alloc(poolConf->nSize, 
                          reserve, 0);

}
    
LOCAL adf_nbuf_t  _buf_pool_dynamic_alloc_buf_align(pool_handle_t handle, BUF_POOL_ID poolId, int reserve, int align)
{
    BUF_POOL_DYNAMIC_CONTEXT *ctx = (BUF_POOL_DYNAMIC_CONTEXT *)handle;
    POOL_CONFIG *poolConf = &ctx->poolConf[poolId];

    return adf_nbuf_alloc(poolConf->nSize, 
                          reserve, align);

}

LOCAL void _buf_pool_dynamic_free_buf(pool_handle_t handle, BUF_POOL_ID poolId, adf_nbuf_t buf)
{
    //BUF_POOL_DYNAMIC_CONTEXT *ctx = (BUF_POOL_DYNAMIC_CONTEXT *)handle;
        
    adf_nbuf_free(buf);
}

adf_os_export_symbol(buf_pool_module_install);
