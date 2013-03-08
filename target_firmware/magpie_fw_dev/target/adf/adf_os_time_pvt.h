#ifndef _ADF_OS_TIME_PVT_H
#define _ADF_OS_TIME_PVT_H

#include <cmnos_api.h>
#include "Magpie_api.h"

/**
 * @brief this code is modified version of tvtohz(9) which
 *        returns signed int which we don't require, hence we
 *        got rid of the type casting thing
 * 
 * @return unsigned long
 */
static inline unsigned long
__adf_os_ticks(void)
{
    return MSEC_TO_TICK(A_MILLISECONDS());
}
static inline a_uint32_t
__adf_os_ticks_to_msecs(unsigned long ticks)
{
	return TICK_TO_MSEC(ticks);
}
static inline unsigned long
__adf_os_msecs_to_ticks(a_uint32_t msecs)
{
	return MSEC_TO_TICK(msecs);
}
static inline unsigned long
__adf_os_getuptime(void)
{
    return MSEC_TO_TICK(A_MILLISECONDS());;      
}

static inline void
__adf_os_udelay(int usecs)
{
    A_DELAY_USECS(usecs);
}

static inline void
__adf_os_mdelay(int msecs)
{
    A_DELAY_USECS(msecs*1000);
}

#endif
