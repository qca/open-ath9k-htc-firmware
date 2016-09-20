/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * Copyright (c) 2016 Oleksij Rempel <linux@rempel-privat.de>
 *
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

#ifndef _ATH_ROM_H_
#define _ATH_ROM_H_

#include <dt_defs.h>
#include <hif_api.h>
#include <htc.h>

int strcmp(const char *s1, const char *s2);

LOCAL BOOLEAN bSet_configuration(void);
LOCAL void HTCControlSvcProcessMsg(HTC_ENDPOINT_ID EndpointID,
		adf_nbuf_t hdr_buf, adf_nbuf_t pBuffers, void *arg);
LOCAL void HTCMsgRecvHandler(adf_nbuf_t hdr_buf,
		adf_nbuf_t buffer, void *context);

void athos_indirection_table_install(void);

void HIFusb_DescTraceDump(void);
void _HIFusb_isr_handler(hif_handle_t);
void _HIFusb_start(hif_handle_t);
void mUsbEPinHighBandSet(uint8_t EPn, uint8_t dir, uint16_t size);
void mUsbEPMap(uint8_t EPn, uint8_t MAP);
void mUsbEPMxPtSzHigh(uint8_t EPn, uint8_t dir, uint16_t size);
void mUsbEPMxPtSzLow(uint8_t EPn, uint8_t dir, uint16_t size);
void mUsbFIFOConfig(uint8_t FIFOn, uint8_t cfg);
void mUsbFIFOMap(uint8_t FIFOn, uint8_t MAP);

#endif /* _ATH_ROM_H_ */
