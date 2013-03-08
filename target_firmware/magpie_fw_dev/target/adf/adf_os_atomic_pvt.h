#ifndef __ADF_OS_ATOMIC_PVT_H
#define __ADF_OS_ATOMIC_PVT_H

//#include <sys/types.h>
//#include <machine/atomic.h>

#include <adf_os_types.h>

typedef a_uint32_t  __adf_os_atomic_t;

/**
 * @brief This initiallizes the varriable to zero
 * 
 * @param __adf_os_atomic_t (int pointer)
 * 
 */
static inline void
__adf_os_atomic_init(__adf_os_atomic_t *v)
{
	//atomic_store_rel_int(v,0);
}
static inline a_uint32_t
__adf_os_atomic_read(__adf_os_atomic_t *v)
{
	//return (atomic_load_acq_int(v));
	return *v;
}

static inline void
__adf_os_atomic_inc(__adf_os_atomic_t *v)
{
	//atomic_add_int(v,1);
	(*v)++;
}

static inline void
__adf_os_atomic_dec(__adf_os_atomic_t *v)
{
	//atomic_subtract_int(v,1);
	(*v)--;
}
/*
static inline void
__adf_os_atomic_write(__adf_os_atomic_t *v,a_uint32_t p)
{
	atomic_store_rel_int(v,(int)p);
} 
 */

#endif
