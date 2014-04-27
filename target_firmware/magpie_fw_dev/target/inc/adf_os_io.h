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
/**
 * @ingroup adf_os_public
 * @file adf_os_io.h
 * This file abstracts I/O operations.
 */

#ifndef _ADF_OS_IO_H
#define _ADF_OS_IO_H

#include <adf_os_io_pvt.h>

/**
 * @brief Read a 32-bit register value
 *
 * @param[in] addr    register addr
 *
 * @return A 32-bit register value.
 */
static inline a_uint32_t ioread32(a_uint32_t addr)
{
	return *(const volatile a_uint32_t *) addr;
}

#define ioread32_mac(addr)	ioread32(WLAN_BASE_ADDRESS + (addr))

/**
 * @brief Write a 32-bit value into register
 *
 * @param[in] osdev   platform device object
 * @param[in] addr    register addr
 * @param[in] l       the 32-bit value to be written
 */
#define adf_os_reg_write32(osdev, addr, l)    __adf_os_reg_write32(osdev, addr, l)

/**
 * @brief Convert a 16-bit value from network byte order to host byte order
 */
#define adf_os_ntohs(x)                         __adf_os_ntohs(x)

/**
 * @brief Convert a 32-bit value from network byte order to host byte order
 */
#define adf_os_ntohl(x)                         __adf_os_ntohl(x)

/**
 * @brief Convert a 16-bit value from host byte order to network byte order
 */
#define adf_os_htons(x)                         __adf_os_htons(x)

/**
 * @brief Convert a 32-bit value from host byte order to network byte order
 */
#define adf_os_htonl(x)                         __adf_os_htonl(x)

/**
 * @brief Convert a 16-bit value from CPU byte order to little-endian byte order
 */
#define adf_os_cpu_to_le16(x)                   __adf_os_cpu_to_le16(x)

#endif
