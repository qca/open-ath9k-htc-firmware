/**
 * @ingroup adf_os_public
 * @file adf_os_defer.h
 * This file abstracts deferred execution contexts.
 */

#ifndef __ADF_OS_DEFER_H
#define __ADF_OS_DEFER_H

#include <adf_os_types.h>
#include <adf_os_defer_pvt.h>

/**
 * TODO This implements work queues (worker threads, kernel threads etc.).
 * Note that there is no cancel on a scheduled work. You cannot free a work 
 * item if its queued. You cannot know if a work item is queued or not unless
 * its running, whence you know its not queued.
 *
 * so if, say, a module is asked to unload itself, how exactly will it make
 * sure that the work's not queued, for OS'es that dont provide such a 
 * mechanism??
 */

/**
 * @brief Representation of a work queue.
 */ 
typedef __adf_os_work_t     adf_os_work_t;

/**
 * @brief Representation of a bottom half.
 */ 
typedef __adf_os_bh_t       adf_os_bh_t;



/**
 * @brief This initiallizes the Bottom half deferred handler
 * 
 * @param[in] hdl   OS handle
 * @param[in] bh    bottom instance
 * @param[in] func  deferred function to run at bottom half interrupt
 *                  context.
 * @param[in] arg   argument for the deferred function
 */
static inline void 
adf_os_init_bh(adf_os_handle_t  hdl, adf_os_bh_t  *bh,
               adf_os_defer_fn_t  func,void  *arg)
{
    __adf_os_init_bh(hdl, bh, func, arg);
}


/**
 * @brief schedule a bottom half (DPC)
 * 
 * @param[in] hdl   OS handle
 * @param[in] bh    bottom instance
 */
static inline void 
adf_os_sched_bh(adf_os_handle_t hdl, adf_os_bh_t *bh)
{
    __adf_os_sched_bh(hdl, bh);
}

/**
 * @brief disable the bh (synchronous)
 * 
 * @param[in] hdl   OS handle
 * @param[in] bh    bottom instance
 */
static inline void 
adf_os_disable_bh(adf_os_handle_t hdl, adf_os_bh_t *bh)
{
    __adf_os_disable_bh(hdl,bh);
}

/*********************Non-Interrupt Context deferred Execution***************/

/**
 * @brief allocate a work/task queue, This runs in non-interrupt
 *        context, so can be preempted by H/W & S/W intr
 * 
 * @param[in] hdl   OS handle
 * @param[in] work  work instance
 * @param[in] func  deferred function to run at bottom half non-interrupt
 *                  context.
 * @param[in] arg   argument for the deferred function
 */
static inline void 
adf_os_init_work(adf_os_handle_t hdl, adf_os_work_t  *work,
                 adf_os_defer_fn_t  func, void  *arg)
{
    __adf_os_init_work(hdl, work, func, arg);
}

/**
 * @brief Schedule a deferred task on non-interrupt context
 * 
 * @param[in] hdl   OS handle
 * @param[in] work  work instance
 */
static inline void 
adf_os_sched_work(adf_os_handle_t  hdl, adf_os_work_t   *work)
{
    __adf_os_sched_work(hdl, work);
}

/**
 *@brief disable the deferred task (synchronous)
 *
 *@param[in] hdl    OS handle
 *@param[in] work   work instance
 */
static inline void 
adf_os_disable_work(adf_os_handle_t hdl, adf_os_work_t *work) 
{
    __adf_os_disable_work(hdl, work);
}


/**
 * XXX API to specify processor while scheduling a bh => only on vista
 */


#endif /*_ADF_OS_DEFER_H*/
