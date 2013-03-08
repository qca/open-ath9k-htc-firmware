#include <adf_os_dma_pvt.h>
#include "Magpie_api.h"
#include "cmnos_api.h"

#if 0
void __adf_os_dma_load(void *arg, bus_dma_segment_t *dseg, int nseg, int error)
{
	if (error)
	   	return;

	adf_os_assert(nseg == 1);

	((bus_dma_segment_t *)arg)[0].ds_addr = dseg[0].ds_addr;
	((bus_dma_segment_t *)arg)[0].ds_len  = dseg[0].ds_len; 
}
#endif

/**
 * @brief Allocates a DMA region, uses the tag elem to store the
 *        tag value which constant for all the mappings done
 *        through this API.
 * 
 * @param osdev
 * @param size
 * @param coherent
 * @param dmap
 * 
 * @return void* (Virtual address)
 */
inline void*
__adf_os_dmamem_alloc(__adf_os_device_t osdev, adf_os_size_t size, 
                      a_bool_t coherent, __adf_os_dma_map_t *dmap)
{    
    (*dmap) = A_ALLOCRAM(sizeof(struct __adf_dma_map));
    
	if((*dmap) == NULL){
		goto fail_malloc;
	}
	    
    (*dmap)->ds_addr = A_ALLOCRAM(size);
    (*dmap)->ds_len = size;
    
    return (*dmap)->ds_addr;
    
fail_malloc: 
    return NULL;            
}
