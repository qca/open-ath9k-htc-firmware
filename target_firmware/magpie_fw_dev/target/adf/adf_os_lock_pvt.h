#ifndef _ADF_OS_LOCK_PVT_H
#define _ADF_OS_LOCK_PVT_H

typedef int  		__adf_os_spinlock_t;
typedef int  		__adf_os_mutex_t;

static inline void __adf_os_init_mutex(__adf_os_mutex_t *mtx)
{
	
}

static inline int __adf_os_mutex_acquire(__adf_os_mutex_t *mtx)
{
	return 0;
}
static inline void __adf_os_mutex_release(__adf_os_mutex_t *mtx)
{

}
static inline void __adf_os_spinlock_init(__adf_os_spinlock_t *lock)
{

}
/*
 * Synchronous versions - only for OS' that have interrupt disable
 */
static inline void   __adf_os_spin_lock_irq(__adf_os_spinlock_t *lock, a_uint32_t    *flags)
{
    //mtx_lock_spin(lock);
    (*flags)=0;
}

static inline void   __adf_os_spin_unlock_irq(__adf_os_spinlock_t *lock, a_uint32_t    *flags)
{
    //mtx_unlock_spin(lock);
}

static inline void		__adf_os_spin_lock_bh(__adf_os_spinlock_t	*lock)
{
	//mtx_lock_spin(lock);
}
static inline void		__adf_os_spin_unlock_bh(__adf_os_spinlock_t	*lock)
{
	//mtx_unlock_spin(lock);
}
static inline a_bool_t __adf_os_spinlock_irq_exec(adf_os_handle_t  hdl, __adf_os_spinlock_t *lock, 
                                                  adf_os_irqlocked_func_t func, void *arg)
{
    return 0;
}
#endif
