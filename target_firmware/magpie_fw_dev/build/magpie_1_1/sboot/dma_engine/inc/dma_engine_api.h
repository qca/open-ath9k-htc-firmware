/*
 * @File: dma_engine_api.h
 * 
 * @Abstract: DMA Engine api
 * 
 * @Notes:
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef _DMA_ENGINE_API_H
#define _DMA_ENGINE_API_H

#include <vbuf_api.h>
#include <vdesc_api.h>

struct zsDmaDesc
{
#if 1   // BIG_ENDIAN
    volatile u16_t      ctrl;       // Descriptor control
    volatile u16_t      status;     // Descriptor status
    volatile u16_t      totalLen;   // Total length
    volatile u16_t      dataSize;   // Data size
#else
    volatile u16_t      status;     // Descriptor status
    volatile u16_t      ctrl;       // Descriptor control
    volatile u16_t      dataSize;   // Data size
    volatile u16_t      totalLen;   // Total length
#endif
    struct zsDmaDesc*   lastAddr;   // Last address of this chain
    volatile u32_t      dataAddr;   // Data buffer address
    struct zsDmaDesc*   nextAddr;   // Next TD address
};

struct zsDmaQueue
{
    struct zsDmaDesc* head;
    struct zsDmaDesc* terminator;
};

// Subclass of zsDmaQueue for TX
struct zsTxDmaQueue
{
    struct zsDmaDesc* head;
    struct zsDmaDesc* terminator;
    
    /* Below are fields specific to TX */
    VBUF *xmited_buf_head;
    VBUF *xmited_buf_tail;        
};

/* hardware API table structure (API descriptions below) */
struct dma_engine_api 
{
    void  (*_init)();

    void  (*_init_rx_queue)(struct zsDmaQueue *q);
    
    void  (*_init_tx_queue)(struct zsTxDmaQueue *q);
                    
    void  (*_config_rx_queue)(struct zsDmaQueue *q, int num_desc, int buf_size);
    
    void  (*_xmit_buf)(struct zsTxDmaQueue *q, VBUF *buf);
    
    void  (*_flush_xmit)(struct zsDmaQueue *q);
    
    VBUF* (*_reap_recv_buf)(struct zsDmaQueue *q);
    
    void  (*_return_recv_buf)(struct zsDmaQueue *q, VBUF *buf);
    
    VBUF* (*_reap_xmited_buf)(struct zsTxDmaQueue *q);
    
    void  (*_swap_data)(struct zsDmaDesc* desc);
    
    int   (*_has_compl_packets)(struct zsDmaQueue *q);
    
    void  (*_desc_dump)(struct zsDmaQueue *q);
    
    /* The functions below are for patchable */
    struct zsDmaDesc* (*_get_packet)(struct zsDmaQueue* q);
    void  (*_reclaim_packet)(struct zsDmaQueue* q, struct zsDmaDesc* desc);
    void (*_put_packet)(struct zsDmaQueue* q, struct zsDmaDesc* desc);
    
    /* room to expand this table by another table */
    void *pReserved;
};

extern void dma_engine_module_install(struct dma_engine_api *apis);

#endif /* #ifndef _DMA_ENGINE_API_H */
