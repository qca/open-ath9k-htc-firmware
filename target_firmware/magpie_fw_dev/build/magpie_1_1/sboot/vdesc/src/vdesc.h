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

#ifndef VDESC_H_
#define VDESC_H_

struct VDESC_CONTEXT {
    VDESC *free_vdesc_head;

    // Left a door for extension the structure
    void *pReserved;
};

#endif /*VDESC_H_*/
