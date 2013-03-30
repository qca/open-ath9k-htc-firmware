/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *  * Neither the name of Qualcomm Atheros nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
