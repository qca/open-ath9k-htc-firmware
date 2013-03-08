/**
 * @ingroup adf_os_public
 * @file adf_os_mem.h
 * This file abstracts memory operations.
 */

#ifndef _ADF_OS_MEM_H
#define _ADF_OS_MEM_H

#include <adf_os_types.h>
#include <adf_os_mem_pvt.h>

/**
 * @brief Allocate a memory buffer. Note this call can block.
 *
 * @param[in] size    buffer size
 *
 * @return Buffer pointer or NULL if there's not enough memory.
 */
static inline void *
adf_os_mem_alloc(adf_os_size_t size)
{
    return __adf_os_mem_alloc(size);
}

/**
 * @brief Free malloc'ed buffer
 *
 * @param[in] buf     buffer pointer allocated by @ref adf_os_mem_alloc
 */
static inline void
adf_os_mem_free(void *buf)
{
    __adf_os_mem_free(buf);
}

/**
 * @brief Move a memory buffer. Overlapping regions are not allowed.
 *
 * @param[in] dst     destination address
 * @param[in] src     source address
 * @param[in] size    buffer size
 */
static inline void
adf_os_mem_copy(void *dst, void *src, adf_os_size_t size)
{
    __adf_os_mem_copy(dst, src, size);
}

/**
 * @brief Does a non-destructive copy of memory buffer
 *
 * @param[in] dst     destination address
 * @param[in] src     source address
 * @param[in] size    buffer size
 */
static inline void 
adf_os_mem_move(void *dst, void *src, adf_os_size_t size)
{
	__adf_os_mem_move(dst,src,size);
}


/**
 * @brief Fill a memory buffer
 * 
 * @param[in] buf   buffer to be filled
 * @param[in] b     byte to fill
 * @param[in] size  buffer size
 */
static inline void
adf_os_mem_set(void *buf, a_uint8_t b, adf_os_size_t size)
{
    __adf_os_mem_set(buf, b, size);
}


/**
 * @brief Zero a memory buffer
 * 
 * @param[in] buf   buffer to be zeroed
 * @param[in] size  buffer size
 */
static inline void
adf_os_mem_zero(void *buf, adf_os_size_t size)
{
    __adf_os_mem_zero(buf, size);
}


/**
 * @brief Compare two memory buffers
 *
 * @param[in] buf1  first buffer
 * @param[in] buf2  second buffer
 * @param[in] size  buffer size
 *
 * @retval    0     equal
 * @retval    1     not equal
 */
static inline int
adf_os_mem_cmp(void *buf1, void *buf2, adf_os_size_t size)
{
    return __adf_os_mem_cmp(buf1, buf2, size);
}

#endif
