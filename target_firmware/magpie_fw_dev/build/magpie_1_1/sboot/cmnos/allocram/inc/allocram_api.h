/*
 * Copyright (c) 2007  Atheros Communications, Inc.  All rights reserved.
 */

#ifndef __ALLOCRAM_API_H__
#define __ALLOCRAM_API_H__

/* API for Target-side startup-time RAM allocations */

struct allocram_api {
    /*
     * Initialize allocram, providing it with a block of RAM
     * (an "arena") from which to allocate.
     *
     * If arena_start is 0, a default start -- the end of
     * the application's text & data -- is used.
     *
     * If arena_sz is 0, a default size -- which uses most
     * of physical RAM beyond arena_start -- is used.
     *
     * Return value is reserved for future use -- it's an arena handle.
     */
    void *(* cmnos_allocram_init)(void *arena_start, A_UINT32 arena_sz);

    /*
     * Allocate nbytes of memory, returning a pointer to the start
     * of the allocated block.  Allocation size is rounded up to the
     * nearest A_CACHE_LINE_SIZE and the returned address similarly
     * aligned.
     *
     * There is no need to check the return value from this function.
     * A failure to satisfy a RAM allocation request is treated as a
     * fatal error.
     *
     * Allocations are expected to occur only during startup; this
     * API does not, for instance, guarantee atomicity with respect
     * to allocations that might (foolishly) be attempted from
     * interrupt handlers.
     *
     * The "which_arena" parameter is currently unused, and should
     * be set to 0 -- only a single arena is currently supported.
     */
    void *(* cmnos_allocram)(void *which_arena, A_UINT32 nbytes);
    
    void (* cmnos_allocram_debug)(void);
};

extern void allocram_module_install(struct allocram_api *api);


#endif /* __ALLOCRAM_API_H__ */
