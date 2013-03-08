/**
 * @defgroup adf_os_public OS abstraction API
 */ 

/**
 * @ingroup adf_os_public
 * @file adf_os_stdtypes.h
 * This file defines standard types.
 */

#ifndef _ADF_OS_STDTYPES_H
#define _ADF_OS_STDTYPES_H

#include <adf_os_types_pvt.h>

/**
 * @brief basic data types. 
 */
typedef enum {
    A_FALSE,
    A_TRUE           
}a_bool_t;

typedef __a_uint8_t    a_uint8_t;
typedef __a_int8_t     a_int8_t;
typedef __a_uint16_t   a_uint16_t;
typedef __a_int16_t    a_int16_t;
typedef __a_uint32_t   a_uint32_t;
typedef __a_int32_t    a_int32_t;
typedef __a_uint64_t   a_uint64_t;
typedef __a_int64_t    a_int64_t;

#endif
