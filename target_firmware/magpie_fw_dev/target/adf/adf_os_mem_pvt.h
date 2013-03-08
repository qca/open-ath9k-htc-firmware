#ifndef ADF_OS_MEM_PVT_H
#define ADF_OS_MEM_PVT_H

#include "cmnos_api.h"
#include "Magpie_api.h"

static inline void *	__adf_os_mem_alloc(adf_os_size_t size)
{
//    return (malloc(size,M_DEVBUF,M_DONTWAIT | M_ZERO));
    return A_ALLOCRAM(size);
}

static inline void 		__adf_os_mem_free(void *buf)
{
    //Should not be called in FW!
    //free(buf,M_DEVBUF);
}

/* move a memory buffer */
static inline void 		__adf_os_mem_copy(void *dst, void *src, adf_os_size_t size)
{
    A_MEMCPY(dst,src,size);    
}

/* set a memory buffer */
static inline void
__adf_os_mem_set(void *buf, a_uint8_t b, adf_os_size_t size)
{
	A_MEMSET(buf, b, size);
}
static inline void
__adf_os_mem_move(void *dst, void *src, adf_os_size_t size)
{
	A_MEMMOVE(dst, src, size);
}
/* zero a memory buffer */
static inline void
__adf_os_mem_zero(void *buf, adf_os_size_t size)
{
    A_MEMZERO(buf,size);
}
/* compare two memory buffers */
static inline int
__adf_os_mem_cmp(void *buf1, void *buf2, adf_os_size_t size)
{
    return (A_MEMCMP (buf1, buf2, size) == 0) ? 0 : 1;
}
#endif
