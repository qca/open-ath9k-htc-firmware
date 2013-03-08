#ifndef __ADF_NBUF_DMA_PVT_H
#define __ADF_NBUF_DMA_PVT_H

#include <adf_os_types.h>
#include <adf_os_util.h>

inline void*
__adf_os_dmamem_alloc(__adf_os_device_t osdev, adf_os_size_t size, 
                      a_bool_t coherent, __adf_os_dma_map_t *dmap);

/* 
 * Free a previously mapped DMA buffer 
 * Direction doesnt matter, since this API is called at closing time.
 */
static inline void
__adf_os_dmamem_free(adf_os_device_t    osdev, __adf_os_size_t size, a_bool_t coherent,
                                         void *vaddr, __adf_os_dma_map_t dmap)
{ 

}


//#define __adf_os_dmamem_map2addr(_dmap)    ((_dmap)->seg[0].ds_addr)
#define __adf_os_dmamem_map2addr(_dmap) ((adf_os_dma_addr_t)(_dmap)->ds_addr)

static inline void 
__adf_os_dmamem_cache_sync(__adf_os_device_t osdev, __adf_os_dma_map_t dmap, adf_os_cache_sync_t sync)
{

}


static inline adf_os_size_t
__adf_os_cache_line_size(void)
{
    /**
     * Todo
     */
   return 0;
}

#endif
