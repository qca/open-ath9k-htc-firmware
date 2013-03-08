/*
 * Copyright (c) 2007-2008 Atheros Communications, Inc.
 * All rights reserved.
 */
#ifndef _ASF_BITMAP_H_
#define _ASF_BITMAP_H_

#include "adf_os_types.h"
#include "adf_os_mem.h"

#define ASF_BYTESZ   8

typedef a_uint8_t * asf_bitmap_t;

/* Bit map related macros. */
// setbit(a,i) ((a)[(i)/NBBY] |= 1<<((i)%NBBY))
// clrbit(a,i) ((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
// isset(a,i)  ((a)[(i)/NBBY] & (1<<((i)%NBBY)))
// isclr(a,i)  (((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

#define asf_howmany(x, y)   (((x)+((y)-1))/(y))
#define asf_roundup(x, y)   ((((x)+((y)-1))/(y))*(y))


static inline a_uint8_t *
asf_bitmap_alloc(int sz_bits)
{
    a_uint8_t * bm;
    int sz_bytes = sz_bits / ASF_BYTESZ;

    bm = adf_os_mem_alloc(sz_bytes);
    if (bm == NULL)
        return NULL;

    adf_os_mem_zero(bm, sz_bytes);
    return bm;
}

static inline void
asf_bitmap_free(a_uint8_t *bm)
{
    adf_os_mem_free(bm);
}

static inline void
asf_bitmap_setbit(a_uint8_t *bm, int pos)
{
    bm[pos / ASF_BYTESZ] |= 1 << (pos % ASF_BYTESZ);
}


static inline void
asf_bitmap_clrbit(a_uint8_t *bm, int pos)
{
    bm[pos / ASF_BYTESZ] &= ~(1 << (pos % ASF_BYTESZ));
}

static inline a_bool_t
asf_bitmap_isset(a_uint8_t *bm, int pos)
{
    return bm[pos / ASF_BYTESZ] & (1 << (pos % ASF_BYTESZ));
}

static inline a_bool_t
asf_bitmap_isclr(a_uint8_t *bm, int pos)
{
    return ((bm[pos / ASF_BYTESZ] & (1 << (pos % ASF_BYTESZ))) == 0);
}

#endif /* _ASF_BITMAP_H */
