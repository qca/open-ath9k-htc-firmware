/**
 * @ingroup adf_os_public
 * @file adf_os_util.h
 * This file defines utility functions.
 */

#ifndef _ADF_OS_UTIL_H
#define _ADF_OS_UTIL_H

#include <adf_os_util_pvt.h>

/**
 * @brief Compiler-dependent macro denoting code likely to execute.
 */
#define adf_os_unlikely(_expr)     __adf_os_unlikely(_expr)

/**
 * @brief Compiler-dependent macro denoting code unlikely to execute.
 */
#define adf_os_likely(_expr)       __adf_os_likely(_expr)

/**
 * @brief read memory barrier. 
 */
#define adf_os_wmb()                __adf_os_wmb()

/**
 * @brief write memory barrier. 
 */
#define adf_os_rmb()                __adf_os_rmb()

/**
 * @brief read + write memory barrier. 
 */
#define adf_os_mb()                 __adf_os_mb()

/**
 * @brief return the lesser of a, b
 */ 
#define adf_os_min(_a, _b)          __adf_os_min(_a, _b)

/**
 * @brief return the larger of a, b
 */ 
#define adf_os_max(_a, _b)          __adf_os_max(_a, _b)

/**
 * @brief assert "expr" evaluates to true.
 */ 
#define adf_os_assert(expr)         __adf_os_assert(expr)

/**
 * @brief supply pseudo-random numbers
 */
static inline void adf_os_get_rand(adf_os_handle_t  hdl, 
                                   a_uint8_t       *ptr, 
                                   a_uint32_t       len)
{
    __adf_os_get_rand(hdl, ptr, len);
}

#endif /*_ADF_OS_UTIL_H*/
