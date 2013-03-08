#ifndef __ADF_OS_UTIL_PVT_H
#define __ADF_OS_UTIL_PVT_H

#include <adf_os_types.h>

#define __adf_os_unlikely(_expr)   
#define __adf_os_likely(_expr)    

/**
 * @brief memory barriers. 
 */
#define __adf_os_wmb()          oops no implementation...
#define __adf_os_rmb()          oops no implementation...
#define __adf_os_mb()           oops no implementation...

#define __adf_os_min(_a, _b)	((_a < _b) ? _a : _b)         
#define __adf_os_max(_a, _b)    ((_a > _b) ? _a : _b)     

#ifdef _DEBUG_BUILD_
#define __adf_os_assert(expr)  do {\
    if(!(expr)) {                                   	\
        adf_os_print("Assertion failed! %s:%s %s:%d\n", #expr, __FUNCTION__, __FILE__, __LINE__);	\
	while(1){}					\
        						\
	}\
}while(0);
#else
#if defined(PROJECT_MAGPIE)
#define __adf_os_assert(expr)  do {      \
    if(!(expr)) {                        \
        adf_os_print("Assertion failed! %s\n", __FUNCTION__);	\
        (*((volatile uint32_t *)(0x12345678)));            \
	}                                    \
}while(0);
#else
#define __adf_os_assert(expr)  do {      \
    if(!(expr)) {                        \
        while(1){}					     \
	}                                    \
}while(0);
#endif
#endif

#ifndef inline
#define inline
#endif

static void inline 	
__adf_os_get_rand(adf_os_handle_t  hdl,__a_uint8_t *ptr, __a_uint32_t len)
{
#if 0    
	u_int8_t *dp = ptr;
	u_int32_t v;
	size_t nb;
	while (len > 0) {
		v = arc4random();
		nb = len > sizeof(u_int32_t) ? sizeof(u_int32_t) : len;
		bcopy(&v, dp, len > sizeof(u_int32_t) ? sizeof(u_int32_t) : len);
		dp += sizeof(u_int32_t);
		len -= nb;
	}
#endif    
}


#endif /*_ADF_OS_UTIL_PVT_H*/
