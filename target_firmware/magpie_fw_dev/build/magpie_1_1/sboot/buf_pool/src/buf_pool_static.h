/*
 * @File: 
 * 
 * @Abstract: 
 * 
 * @Notes: 
 * 
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef BUF_POOL_STATIC_H_
#define BUF_POOL_STATIC_H_

typedef struct _BUF_POOL_STATIC_CONTEXT {
    VBUF *bufQ[POOL_ID_MAX];
    
    // Left a door for extension the structure
    void *pReserved;
} BUF_POOL_STATIC_CONTEXT;

#endif /*BUF_POOL_STATIC_H_*/
