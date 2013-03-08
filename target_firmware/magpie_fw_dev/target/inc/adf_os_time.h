/**
 * @ingroup adf_os_public
 * @file adf_os_time.h
 * This file abstracts time related functionality.
 */
#ifndef _ADF_OS_TIME_H
#define _ADF_OS_TIME_H

#include <adf_os_time_pvt.h>

/**
 * @brief count the number of ticks elapsed from the time when
 *        the system booted
 * 
 * @return ticks
 */
static inline unsigned long
adf_os_ticks(void)
{
	return __adf_os_ticks();
}

/**
 * @brief convert ticks to milliseconds
 *
 * @param[in] ticks number of ticks
 * @return time in milliseconds
 */ 
static inline a_uint32_t
adf_os_ticks_to_msecs(unsigned long ticks)
{
	return (__adf_os_ticks_to_msecs(ticks));
}

/**
 * @brief convert milliseconds to ticks
 *
 * @param[in] time in milliseconds
 * @return number of ticks
 */ 
static inline unsigned long
adf_os_msecs_to_ticks(a_uint32_t msecs)
{
	return (__adf_os_msecs_to_ticks(msecs));
}

/**
 * @brief Return a monotonically increasing time. This increments once per HZ ticks
 */
static inline unsigned long
adf_os_getuptime(void)
{
    return (__adf_os_getuptime());
}

/**
 * @brief Delay in microseconds
 *
 * @param[in] microseconds to delay
 */
static inline void
adf_os_udelay(int usecs)
{
    __adf_os_udelay(usecs);
}

/**
 * @brief Delay in milliseconds.
 *
 * @param[in] milliseconds to delay
 */
static inline void
adf_os_mdelay(int msecs)
{
    __adf_os_mdelay(msecs);
}

/**
 * @brief Check if _a is later than _b.
 */ 
#define adf_os_time_after(_a, _b)       __adf_os_time_after(_a, _b)

/**
 * @brief Check if _a is prior to _b.
 */ 
#define adf_os_time_before(_a, _b)      __adf_os_time_before(_a, _b)

/**
 * @brief Check if _a atleast as recent as _b, if not later.
 */ 
#define adf_os_time_after_eq(_a, _b)    __adf_os_time_after_eq(_a, _b)

#endif
    

