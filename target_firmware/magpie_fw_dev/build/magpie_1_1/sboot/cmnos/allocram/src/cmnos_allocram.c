/*
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 */

#include "sys_cfg.h"
#include "athos_api.h"

#if SYSTEM_MODULE_ALLOCRAM

/*
 * Startup time RAM allocation.
 *
 * Oddly enough, we allow allocation, but not free.
 * The central idea is to restrict compile-time RAM demands
 * of modules to a minimum so that if a module is replaced
 * at run-time large amounts of RAM are not wasted.
 *
 * Addresses returned are A_CACHE_LINE_SIZE aligned.
 */

LOCAL A_UINT32 allocram_current_addr;
LOCAL A_UINT32 allocram_remaining_bytes;

LOCAL void *
cmnos_allocram_init(void *arena_start, A_UINT32 arena_sz)
{
    A_UINT32 astart = (A_UINT32)arena_start;

#if defined(__XTENSA__)
    /*
     * This hacky line converts from a text or data RAM address
     * into a data RAM address.  (It's all the same on MIPS, but
     * text and data are different address spaces on Xtensa.)
     */
    //astart = TARG_RAM_ADDRS(TARG_RAM_OFFSET(astart));
#endif

#if 0
    if (arena_sz == 0) {
        /* Default arena_sz to most of available RAM */
        arena_sz = TARG_RAM_SZ - (A_UINT32)TARG_RAM_OFFSET(astart);
        arena_sz -= HOST_INTEREST->hi_end_RAM_reserve_sz;
    }
#endif

    /* Clear entire area */
//    A_MEMSET(astart, 0, arena_sz);

    /* Adjust for cache line alignment */
#if 0    
    allocram_current_addr = A_ROUND_UP(astart, A_CACHE_LINE_SIZE);
    arena_sz -= (allocram_current_addr-astart);
#else
    allocram_current_addr = astart;
#endif    
    allocram_remaining_bytes = arena_sz;

    //A_DCACHE_FLUSH();

    //A_PRINTF("cmnos_allocram_init: start=0x%x size=%d\n",
    //    allocram_current_addr, allocram_remaining_bytes);

    return NULL; /* Future implementation may return an arena handle */
}

/*
 * Allocate nbytes from the arena.  At this point, which_arena should
 * be set to 0 for the default (and only) arena.  A future allocation
 * module may support multiple separate arenas.
 */
LOCAL void *
cmnos_allocram(void * which_arena, A_UINT32 nbytes)
{
    void *ptr = (void *)allocram_current_addr;
    //nbytes = A_ROUND_UP(nbytes, A_CACHE_LINE_SIZE);
    nbytes = A_ROUND_UP(nbytes, 4);
    if (nbytes <= allocram_remaining_bytes) {
        allocram_remaining_bytes -= nbytes;
        allocram_current_addr += nbytes;
    } else {
        A_PRINTF("RAM allocation (%d bytes) failed!\n", nbytes);
        //A_ASSERT(0);
        adf_os_assert(0);
    }

    return ptr;
}

void
cmnos_allocram_debug(void)
{
    A_PRINTF("ALLOCRAM Current Addr 0x%x\n", allocram_current_addr);
    A_PRINTF("ALLOCRAM Remaining Bytes %d\n", allocram_remaining_bytes);
}

void
cmnos_allocram_module_install(struct allocram_api *tbl)
{
    tbl->cmnos_allocram_init                 = cmnos_allocram_init;
    tbl->cmnos_allocram                      = cmnos_allocram;
    tbl->cmnos_allocram_debug                = cmnos_allocram_debug;
}

#endif /* SYSTEM_MODULE_ALLOCRAM */

