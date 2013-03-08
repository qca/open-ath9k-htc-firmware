#ifndef __ADF_OS_DEFER_PVT_H
#define __ADF_OS_DEFER_PVT_H

#include <adf_os_types.h>
#include <cmnos_api.h>
#include "Magpie_api.h"

/*
 * Because the real function taked an extra int :(
 */
typedef struct {
    adf_os_defer_fn_t  caller_fn;
    void             *caller_arg;
}__adf_os_defer_ctx_t;

/*
 * wrapper around the real task func
 */
typedef struct {
    //struct task          tsk;
    __adf_os_defer_ctx_t ctx;
}__adf_os_defer_t;

//typedef __adf_os_defer_t    __adf_os_bh_t;
typedef A_tasklet_t         __adf_os_bh_t;
typedef __adf_os_defer_t    __adf_os_work_t;

/*
 * wrapper function
 */
extern void __adf_os_defer_func(void *arg, int pending);

/**
 * @brief initiallize the defer function (work or bh)
 * 
 * @param defer
 * @param func
 * @param arg
 */
static inline void __adf_os_init_defer(__adf_os_defer_t  *defer,
                                       adf_os_defer_fn_t    func, 
                                       void              *arg)
{
    defer->ctx.caller_fn  = func;
    defer->ctx.caller_arg = arg;

    //TASK_INIT(&defer->tsk, 0, __adf_os_defer_func, &defer->ctx);
}

static inline void __adf_os_init_work(adf_os_handle_t  hdl,
									  __adf_os_work_t	*work,
									  adf_os_defer_fn_t	func,
									  void 				*arg)
{
	__adf_os_init_defer(work, func, arg);
}

static inline void	__adf_os_init_bh(adf_os_handle_t  hdl,
									 __adf_os_bh_t		*bh,
									 adf_os_defer_fn_t	func,
									 void				*arg)
{
	//__adf_os_init_defer(bh, func, arg);
	A_TASKLET_INIT_TASK(func, arg, bh);
}
static inline void __adf_os_sched_work(adf_os_handle_t  hdl, 
                                       __adf_os_work_t  * work)
{
    //taskqueue_enqueue(taskqueue_thread, &work->tsk);
}
static inline void __adf_os_disable_work(adf_os_handle_t  hdl, 
                                         __adf_os_work_t  * work)
{
    //taskqueue_drain(taskqueue_thread, &work->tsk);
}

static inline void __adf_os_sched_bh(adf_os_handle_t  hdl, 
                                       __adf_os_bh_t  * bh)
{
    A_TASKLET_SCHEDULE(bh);
}

static inline void __adf_os_disable_bh(adf_os_handle_t  hdl, 
                                       __adf_os_bh_t  * bh)
{
    A_TASKLET_DISABLE(bh);
}

#endif 
