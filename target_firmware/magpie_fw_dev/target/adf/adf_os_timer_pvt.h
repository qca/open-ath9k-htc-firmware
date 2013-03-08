#ifndef _ADF_OS_TIMER_PVT_H
#define _ADF_OS_TIMER_PVT_H

#include <cmnos_api.h>
#include "Magpie_api.h"


typedef struct 
{
	A_timer_t 				*magpie_timer;
	adf_os_timer_func_t 	timer_func;
}__adf_os_timer_t;

//typedef A_timer_t 	__adf_os_timer_t;

void
__adf_os_timer_func(A_HANDLE timer_handle, void *arg);

/* 
 * Initialize a timer
 */
static inline void
__adf_os_timer_init(adf_os_handle_t  hdl, __adf_os_timer_t   *timer,
                    adf_os_timer_func_t  func, void *arg)
{
    timer->timer_func = func;
    A_INIT_TIMER(timer->magpie_timer, __adf_os_timer_func, arg);
}

/* 
 * start a timer 
 */
static inline void
__adf_os_timer_start(__adf_os_timer_t *timer, int msec)
{
    A_TIMEOUT_MS(timer->magpie_timer, msec);
}
/*
 * Cancel a timer
 *
 * Return: TRUE if timer was cancelled and deactived,
 *         FALSE if timer was cancelled but already got fired.
 */
static inline a_bool_t
__adf_os_timer_cancel(__adf_os_timer_t *timer)
{
	A_UNTIMEOUT(timer->magpie_timer);
	return A_TRUE;
}

/*
 * XXX Synchronously canel a timer
 *
 * Return: TRUE if timer was cancelled and deactived,
 *         FALSE if timer was cancelled but already got fired.
 *
 * Synchronization Rules:
 * 1. caller must make sure timer function will not use
 *    adf_os_set_timer to add iteself again.
 * 2. caller must not hold any lock that timer function
 *    is likely to hold as well.
 * 3. It can't be called from interrupt context.
 */
static inline a_bool_t
__adf_os_timer_sync_cancel(__adf_os_timer_t *timer)
{
    // @TODO: IS OK??
    A_UNTIMEOUT(timer->magpie_timer);
    return A_TRUE;
}


#endif
