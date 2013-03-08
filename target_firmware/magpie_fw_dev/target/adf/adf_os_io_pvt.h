#ifndef _ADF_OS_IO_PVT_H
#define _ADF_OS_IO_PVT_H

#include <reg_defs.h>

#define __bswap16(x)                                \
	                                                ((((x) & 0xff00) >> 8) | \
	                                                 (((x) & 0x00ff) << 8))

#define __bswap32(x)                                \
	                                                ((((x) & 0xff000000) >> 24) | \
                                                	 (((x) & 0x00ff0000) >>  8) | \
                                                	 (((x) & 0x0000ff00) <<  8) | \
                                                	 (((x) & 0x000000ff) << 24))

#if 0
static __inline __uint64_t
__bswap64(__uint64_t _x)
{

	return ((_x >> 56) | ((_x >> 40) & 0xff00) | ((_x >> 24) & 0xff0000) |
	    ((_x >> 8) & 0xff000000) | ((_x << 8) & ((__uint64_t)0xff << 32)) |
	    ((_x << 24) & ((__uint64_t)0xff << 40)) |
	    ((_x << 40) & ((__uint64_t)0xff << 48)) | ((_x << 56)));
}
#endif
	 	                     
#define __adf_os_reg_read8(_dev, _addr)		        oops no implementation...     

#define __adf_os_reg_read16(_dev, _addr)            oops no implementation...

#define __adf_os_reg_read32(_dev, _addr)            *((a_uint32_t *)(WLAN_BASE_ADDRESS + _addr))

#define __adf_os_reg_read64(_dev, _addr)            oops no implementation...

#define __adf_os_reg_write8(_dev, _addr, _val)      oops no implementation...

#define __adf_os_reg_write16(_dev, _addr, _val)     oops no implementation...

// RAY: The delay is required. But needs to know why??
//#define __adf_os_reg_write32(_dev, _addr, _val)     *((a_uint32_t *)(0x10ff0000 + _addr)) = _val; A_DELAY_USECS(100);
//RAY 0806: Move the delay (100us) to the host side because of throughput issue
#define __adf_os_reg_write32(_dev, _addr, _val)     *((a_uint32_t *)(WLAN_BASE_ADDRESS + _addr)) = _val;

#define __adf_os_reg_write64(_dev, _addr, _val)     oops no implementation...

#define __adf_os_ntohs(x)                    	    x
#define __adf_os_ntohl(x)                    	    x

#define __adf_os_htons(x)                    	    x
#define __adf_os_htonl(x)                    	    x

#define __adf_os_cpu_to_le16(x)		                __bswap16(x)			
#define __adf_os_cpu_to_le32(x)			            __bswap32(x)
#define __adf_os_cpu_to_le64(x)                     x     	

#define __adf_os_cpu_to_be16(x)                     x     	
#define __adf_os_cpu_to_be32(x)                     x     	
#define __adf_os_cpu_to_be64(x)                     x     	

#define __adf_os_le16_to_cpu(x)                     __bswap16(x)
#define __adf_os_le32_to_cpu(x)                     __bswap32(x)    	
#define __adf_os_le64_to_cpu(x)                     x     	

#define __adf_os_be16_to_cpu(x)                     x     	
#define __adf_os_be32_to_cpu(x)                     x     	
#define __adf_os_be64_to_cpu(x)                     x     	

#endif
