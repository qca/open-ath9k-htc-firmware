/*
 * @File: vbuf.c
 * 
 * @Abstract: 
 * 
 * @Notes:
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 */


#include <osapi.h>
#include <vbuf_api.h>
#include <Magpie_api.h>

#include "vbuf.h"

#define VBUF_SIZE           sizeof(VBUF)

struct VBUF_CONTEXT g_vbufCtx;

void _vbuf_init(int nBuf);
VBUF* _vbuf_alloc_vbuf(void);
void _vbuf_free_vbuf(VBUF *buf);

void _vbuf_init(int nBuf) 
{
    int i;
    VBUF *vbuf;
    
    //vbuf = (VBUF*)dataAddr;
    vbuf = (VBUF*)A_ALLOCRAM(VBUF_SIZE);
    vbuf->next_buf = NULL;
    vbuf->desc_list = NULL;
                    
    g_vbufCtx.free_buf_head = vbuf;
    
    for(i=1; i<nBuf; i++)
    {
        //vbuf = (VBUF*)(dataAddr + i*VBUF_SIZE);
        vbuf = (VBUF*)A_ALLOCRAM(VBUF_SIZE);
        
        vbuf->desc_list = NULL;
        vbuf->next_buf = g_vbufCtx.free_buf_head;
        g_vbufCtx.free_buf_head = vbuf;
    }    
    
    g_vbufCtx.nVbufNum = nBuf;
    //return (dataAddr + nBuf*VBUF_SIZE);
    return;
}

VBUF* _vbuf_alloc_vbuf(void)
{
    VBUF *allocBuf = NULL;
    
    if ( g_vbufCtx.free_buf_head != NULL )
    {
        allocBuf = g_vbufCtx.free_buf_head;
        g_vbufCtx.nVbufNum--;
        
        g_vbufCtx.free_buf_head = allocBuf->next_buf;
        allocBuf->next_buf = NULL;        
    }
    
    return allocBuf;
}

void _vbuf_free_vbuf(VBUF *buf)
{
    // assert buf != NULL
    
    buf->next_buf = g_vbufCtx.free_buf_head;
    g_vbufCtx.free_buf_head = buf;
    
    g_vbufCtx.nVbufNum++;
}

/* the exported entry point into this module. All apis are accessed through
 * function pointers */
void vbuf_module_install(struct vbuf_api *apis)
{    
        /* hook in APIs */
    apis->_init = _vbuf_init;
    apis->_alloc_vbuf = _vbuf_alloc_vbuf;
    apis->_free_vbuf = _vbuf_free_vbuf;
    
        /* save ptr to the ptr to the context for external code to inspect/modify internal module state */
    //apis->pReserved = &g_pMboxHWContext;
}
 

