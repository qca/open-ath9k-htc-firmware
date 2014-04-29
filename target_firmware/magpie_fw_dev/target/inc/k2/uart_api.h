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
/*************************************************************************/
/*  Copyright (c) 2006 Atheros Communications, Inc., All Rights Reserved */
/*                                                                       */
/*  Module Name : uart.h     	                                         */
/*                                                                       */
/*  Abstract                                                             */
/*      This file contains definition of uart registers, marco and api.  */
/*                                                                       */
/*  NOTES                                                                */
/*      None                                                             */
/*                                                                       */
/*************************************************************************/

#ifndef _UART_API_H_
#define _UART_API_H_

#include "k2/reg_defs.h"

/************************** Register deinition ***************************/
#define RBR_ADDRESS                              0x00051000
#define RBR_OFFSET                               0x00000000
#define RBR_RBR_MSB                              7
#define RBR_RBR_LSB                              0
#define RBR_RBR_MASK                             0x000000ff
#define RBR_RBR_GET(x)                           (((x) & RBR_RBR_MASK) >> RBR_RBR_LSB)
#define RBR_RBR_SET(x)                           (((x) << RBR_RBR_LSB) & RBR_RBR_MASK)

#define THR_ADDRESS                              0x00051000
#define THR_OFFSET                               0x00000000
#define THR_THR_MSB                              7
#define THR_THR_LSB                              0
#define THR_THR_MASK                             0x000000ff
#define THR_THR_GET(x)                           (((x) & THR_THR_MASK) >> THR_THR_LSB)
#define THR_THR_SET(x)                           (((x) << THR_THR_LSB) & THR_THR_MASK)

#define DLL_ADDRESS                              0x00051000
#define DLL_OFFSET                               0x00000000
#define DLL_DLL_MSB                              7
#define DLL_DLL_LSB                              0
#define DLL_DLL_MASK                             0x000000ff
#define DLL_DLL_GET(x)                           (((x) & DLL_DLL_MASK) >> DLL_DLL_LSB)
#define DLL_DLL_SET(x)                           (((x) << DLL_DLL_LSB) & DLL_DLL_MASK)

#define DLH_ADDRESS                              0x00051004
#define DLH_OFFSET                               0x00000004
#define DLH_DLH_MSB                              7
#define DLH_DLH_LSB                              0
#define DLH_DLH_MASK                             0x000000ff
#define DLH_DLH_GET(x)                           (((x) & DLH_DLH_MASK) >> DLH_DLH_LSB)
#define DLH_DLH_SET(x)                           (((x) << DLH_DLH_LSB) & DLH_DLH_MASK)

#define IER_ADDRESS                              0x00051004
#define IER_OFFSET                               0x00000004
#define IER_EDDSI_MSB                            3
#define IER_EDDSI_LSB                            3
#define IER_EDDSI_MASK                           0x00000008
#define IER_EDDSI_GET(x)                         (((x) & IER_EDDSI_MASK) >> IER_EDDSI_LSB)
#define IER_EDDSI_SET(x)                         (((x) << IER_EDDSI_LSB) & IER_EDDSI_MASK)
#define IER_ELSI_MSB                             2
#define IER_ELSI_LSB                             2
#define IER_ELSI_MASK                            0x00000004
#define IER_ELSI_GET(x)                          (((x) & IER_ELSI_MASK) >> IER_ELSI_LSB)
#define IER_ELSI_SET(x)                          (((x) << IER_ELSI_LSB) & IER_ELSI_MASK)
#define IER_ETBEI_MSB                            1
#define IER_ETBEI_LSB                            1
#define IER_ETBEI_MASK                           0x00000002
#define IER_ETBEI_GET(x)                         (((x) & IER_ETBEI_MASK) >> IER_ETBEI_LSB)
#define IER_ETBEI_SET(x)                         (((x) << IER_ETBEI_LSB) & IER_ETBEI_MASK)
#define IER_ERBFI_MSB                            0
#define IER_ERBFI_LSB                            0
#define IER_ERBFI_MASK                           0x00000001
#define IER_ERBFI_GET(x)                         (((x) & IER_ERBFI_MASK) >> IER_ERBFI_LSB)
#define IER_ERBFI_SET(x)                         (((x) << IER_ERBFI_LSB) & IER_ERBFI_MASK)

#define IIR_ADDRESS                              0x00051008
#define IIR_OFFSET                               0x00000008
#define IIR_FIFO_STATUS_MSB                      7
#define IIR_FIFO_STATUS_LSB                      6
#define IIR_FIFO_STATUS_MASK                     0x000000c0
#define IIR_FIFO_STATUS_GET(x)                   (((x) & IIR_FIFO_STATUS_MASK) >> IIR_FIFO_STATUS_LSB)
#define IIR_FIFO_STATUS_SET(x)                   (((x) << IIR_FIFO_STATUS_LSB) & IIR_FIFO_STATUS_MASK)
#define IIR_IID_MSB                              3
#define IIR_IID_LSB                              0
#define IIR_IID_MASK                             0x0000000f
#define IIR_IID_GET(x)                           (((x) & IIR_IID_MASK) >> IIR_IID_LSB)
#define IIR_IID_SET(x)                           (((x) << IIR_IID_LSB) & IIR_IID_MASK)

#define FCR_ADDRESS                              0x00051008
#define FCR_OFFSET                               0x00000008
#define FCR_RCVR_TRIG_MSB                        7
#define FCR_RCVR_TRIG_LSB                        6
#define FCR_RCVR_TRIG_MASK                       0x000000c0
#define FCR_RCVR_TRIG_GET(x)                     (((x) & FCR_RCVR_TRIG_MASK) >> FCR_RCVR_TRIG_LSB)
#define FCR_RCVR_TRIG_SET(x)                     (((x) << FCR_RCVR_TRIG_LSB) & FCR_RCVR_TRIG_MASK)
#define FCR_DMA_MODE_MSB                         3
#define FCR_DMA_MODE_LSB                         3
#define FCR_DMA_MODE_MASK                        0x00000008
#define FCR_DMA_MODE_GET(x)                      (((x) & FCR_DMA_MODE_MASK) >> FCR_DMA_MODE_LSB)
#define FCR_DMA_MODE_SET(x)                      (((x) << FCR_DMA_MODE_LSB) & FCR_DMA_MODE_MASK)
#define FCR_XMIT_FIFO_RST_MSB                    2
#define FCR_XMIT_FIFO_RST_LSB                    2
#define FCR_XMIT_FIFO_RST_MASK                   0x00000004
#define FCR_XMIT_FIFO_RST_GET(x)                 (((x) & FCR_XMIT_FIFO_RST_MASK) >> FCR_XMIT_FIFO_RST_LSB)
#define FCR_XMIT_FIFO_RST_SET(x)                 (((x) << FCR_XMIT_FIFO_RST_LSB) & FCR_XMIT_FIFO_RST_MASK)
#define FCR_RCVR_FIFO_RST_MSB                    1
#define FCR_RCVR_FIFO_RST_LSB                    1
#define FCR_RCVR_FIFO_RST_MASK                   0x00000002
#define FCR_RCVR_FIFO_RST_GET(x)                 (((x) & FCR_RCVR_FIFO_RST_MASK) >> FCR_RCVR_FIFO_RST_LSB)
#define FCR_RCVR_FIFO_RST_SET(x)                 (((x) << FCR_RCVR_FIFO_RST_LSB) & FCR_RCVR_FIFO_RST_MASK)
#define FCR_FIFO_EN_MSB                          0
#define FCR_FIFO_EN_LSB                          0
#define FCR_FIFO_EN_MASK                         0x00000001
#define FCR_FIFO_EN_GET(x)                       (((x) & FCR_FIFO_EN_MASK) >> FCR_FIFO_EN_LSB)
#define FCR_FIFO_EN_SET(x)                       (((x) << FCR_FIFO_EN_LSB) & FCR_FIFO_EN_MASK)

#define LCR_ADDRESS                              0x0005100c
#define LCR_OFFSET                               0x0000000c
#define LCR_DLAB_MSB                             7
#define LCR_DLAB_LSB                             7
#define LCR_DLAB_MASK                            0x00000080
#define LCR_DLAB_GET(x)                          (((x) & LCR_DLAB_MASK) >> LCR_DLAB_LSB)
#define LCR_DLAB_SET(x)                          (((x) << LCR_DLAB_LSB) & LCR_DLAB_MASK)
#define LCR_BREAK_MSB                            6
#define LCR_BREAK_LSB                            6
#define LCR_BREAK_MASK                           0x00000040
#define LCR_BREAK_GET(x)                         (((x) & LCR_BREAK_MASK) >> LCR_BREAK_LSB)
#define LCR_BREAK_SET(x)                         (((x) << LCR_BREAK_LSB) & LCR_BREAK_MASK)
#define LCR_EPS_MSB                              4
#define LCR_EPS_LSB                              4
#define LCR_EPS_MASK                             0x00000010
#define LCR_EPS_GET(x)                           (((x) & LCR_EPS_MASK) >> LCR_EPS_LSB)
#define LCR_EPS_SET(x)                           (((x) << LCR_EPS_LSB) & LCR_EPS_MASK)
#define LCR_PEN_MSB                              3
#define LCR_PEN_LSB                              3
#define LCR_PEN_MASK                             0x00000008
#define LCR_PEN_GET(x)                           (((x) & LCR_PEN_MASK) >> LCR_PEN_LSB)
#define LCR_PEN_SET(x)                           (((x) << LCR_PEN_LSB) & LCR_PEN_MASK)
#define LCR_STOP_MSB                             2
#define LCR_STOP_LSB                             2
#define LCR_STOP_MASK                            0x00000004
#define LCR_STOP_GET(x)                          (((x) & LCR_STOP_MASK) >> LCR_STOP_LSB)
#define LCR_STOP_SET(x)                          (((x) << LCR_STOP_LSB) & LCR_STOP_MASK)
#define LCR_CLS_MSB                              1
#define LCR_CLS_LSB                              0
#define LCR_CLS_MASK                             0x00000003
#define LCR_CLS_GET(x)                           (((x) & LCR_CLS_MASK) >> LCR_CLS_LSB)
#define LCR_CLS_SET(x)                           (((x) << LCR_CLS_LSB) & LCR_CLS_MASK)

#define MCR_ADDRESS                              0x00051010
#define MCR_OFFSET                               0x00000010
#define MCR_LOOPBACK_MSB                         5
#define MCR_LOOPBACK_LSB                         5
#define MCR_LOOPBACK_MASK                        0x00000020
#define MCR_LOOPBACK_GET(x)                      (((x) & MCR_LOOPBACK_MASK) >> MCR_LOOPBACK_LSB)
#define MCR_LOOPBACK_SET(x)                      (((x) << MCR_LOOPBACK_LSB) & MCR_LOOPBACK_MASK)
#define MCR_OUT2_MSB                             3
#define MCR_OUT2_LSB                             3
#define MCR_OUT2_MASK                            0x00000008
#define MCR_OUT2_GET(x)                          (((x) & MCR_OUT2_MASK) >> MCR_OUT2_LSB)
#define MCR_OUT2_SET(x)                          (((x) << MCR_OUT2_LSB) & MCR_OUT2_MASK)
#define MCR_OUT1_MSB                             2
#define MCR_OUT1_LSB                             2
#define MCR_OUT1_MASK                            0x00000004
#define MCR_OUT1_GET(x)                          (((x) & MCR_OUT1_MASK) >> MCR_OUT1_LSB)
#define MCR_OUT1_SET(x)                          (((x) << MCR_OUT1_LSB) & MCR_OUT1_MASK)
#define MCR_RTS_MSB                              1
#define MCR_RTS_LSB                              1
#define MCR_RTS_MASK                             0x00000002
#define MCR_RTS_GET(x)                           (((x) & MCR_RTS_MASK) >> MCR_RTS_LSB)
#define MCR_RTS_SET(x)                           (((x) << MCR_RTS_LSB) & MCR_RTS_MASK)
#define MCR_DTR_MSB                              0
#define MCR_DTR_LSB                              0
#define MCR_DTR_MASK                             0x00000001
#define MCR_DTR_GET(x)                           (((x) & MCR_DTR_MASK) >> MCR_DTR_LSB)
#define MCR_DTR_SET(x)                           (((x) << MCR_DTR_LSB) & MCR_DTR_MASK)

#define LSR_ADDRESS                              0x00051014
#define LSR_OFFSET                               0x00000014
#define LSR_FERR_MSB                             7
#define LSR_FERR_LSB                             7
#define LSR_FERR_MASK                            0x00000080
#define LSR_FERR_GET(x)                          (((x) & LSR_FERR_MASK) >> LSR_FERR_LSB)
#define LSR_FERR_SET(x)                          (((x) << LSR_FERR_LSB) & LSR_FERR_MASK)
#define LSR_TEMT_MSB                             6
#define LSR_TEMT_LSB                             6
#define LSR_TEMT_MASK                            0x00000040
#define LSR_TEMT_GET(x)                          (((x) & LSR_TEMT_MASK) >> LSR_TEMT_LSB)
#define LSR_TEMT_SET(x)                          (((x) << LSR_TEMT_LSB) & LSR_TEMT_MASK)
#define LSR_THRE_MSB                             5
#define LSR_THRE_LSB                             5
#define LSR_THRE_MASK                            0x00000020
#define LSR_THRE_GET(x)                          (((x) & LSR_THRE_MASK) >> LSR_THRE_LSB)
#define LSR_THRE_SET(x)                          (((x) << LSR_THRE_LSB) & LSR_THRE_MASK)
#define LSR_BI_MSB                               4
#define LSR_BI_LSB                               4
#define LSR_BI_MASK                              0x00000010
#define LSR_BI_GET(x)                            (((x) & LSR_BI_MASK) >> LSR_BI_LSB)
#define LSR_BI_SET(x)                            (((x) << LSR_BI_LSB) & LSR_BI_MASK)
#define LSR_FE_MSB                               3
#define LSR_FE_LSB                               3
#define LSR_FE_MASK                              0x00000008
#define LSR_FE_GET(x)                            (((x) & LSR_FE_MASK) >> LSR_FE_LSB)
#define LSR_FE_SET(x)                            (((x) << LSR_FE_LSB) & LSR_FE_MASK)
#define LSR_PE_MSB                               2
#define LSR_PE_LSB                               2
#define LSR_PE_MASK                              0x00000004
#define LSR_PE_GET(x)                            (((x) & LSR_PE_MASK) >> LSR_PE_LSB)
#define LSR_PE_SET(x)                            (((x) << LSR_PE_LSB) & LSR_PE_MASK)
#define LSR_OE_MSB                               1
#define LSR_OE_LSB                               1
#define LSR_OE_MASK                              0x00000002
#define LSR_OE_GET(x)                            (((x) & LSR_OE_MASK) >> LSR_OE_LSB)
#define LSR_OE_SET(x)                            (((x) << LSR_OE_LSB) & LSR_OE_MASK)
#define LSR_DR_MSB                               0
#define LSR_DR_LSB                               0
#define LSR_DR_MASK                              0x00000001
#define LSR_DR_GET(x)                            (((x) & LSR_DR_MASK) >> LSR_DR_LSB)
#define LSR_DR_SET(x)                            (((x) << LSR_DR_LSB) & LSR_DR_MASK)

#define MSR_ADDRESS                              0x00051018
#define MSR_OFFSET                               0x00000018
#define MSR_DCD_MSB                              7
#define MSR_DCD_LSB                              7
#define MSR_DCD_MASK                             0x00000080
#define MSR_DCD_GET(x)                           (((x) & MSR_DCD_MASK) >> MSR_DCD_LSB)
#define MSR_DCD_SET(x)                           (((x) << MSR_DCD_LSB) & MSR_DCD_MASK)
#define MSR_RI_MSB                               6
#define MSR_RI_LSB                               6
#define MSR_RI_MASK                              0x00000040
#define MSR_RI_GET(x)                            (((x) & MSR_RI_MASK) >> MSR_RI_LSB)
#define MSR_RI_SET(x)                            (((x) << MSR_RI_LSB) & MSR_RI_MASK)
#define MSR_DSR_MSB                              5
#define MSR_DSR_LSB                              5
#define MSR_DSR_MASK                             0x00000020
#define MSR_DSR_GET(x)                           (((x) & MSR_DSR_MASK) >> MSR_DSR_LSB)
#define MSR_DSR_SET(x)                           (((x) << MSR_DSR_LSB) & MSR_DSR_MASK)
#define MSR_CTS_MSB                              4
#define MSR_CTS_LSB                              4
#define MSR_CTS_MASK                             0x00000010
#define MSR_CTS_GET(x)                           (((x) & MSR_CTS_MASK) >> MSR_CTS_LSB)
#define MSR_CTS_SET(x)                           (((x) << MSR_CTS_LSB) & MSR_CTS_MASK)
#define MSR_DDCD_MSB                             3
#define MSR_DDCD_LSB                             3
#define MSR_DDCD_MASK                            0x00000008
#define MSR_DDCD_GET(x)                          (((x) & MSR_DDCD_MASK) >> MSR_DDCD_LSB)
#define MSR_DDCD_SET(x)                          (((x) << MSR_DDCD_LSB) & MSR_DDCD_MASK)
#define MSR_TERI_MSB                             2
#define MSR_TERI_LSB                             2
#define MSR_TERI_MASK                            0x00000004
#define MSR_TERI_GET(x)                          (((x) & MSR_TERI_MASK) >> MSR_TERI_LSB)
#define MSR_TERI_SET(x)                          (((x) << MSR_TERI_LSB) & MSR_TERI_MASK)
#define MSR_DDSR_MSB                             1
#define MSR_DDSR_LSB                             1
#define MSR_DDSR_MASK                            0x00000002
#define MSR_DDSR_GET(x)                          (((x) & MSR_DDSR_MASK) >> MSR_DDSR_LSB)
#define MSR_DDSR_SET(x)                          (((x) << MSR_DDSR_LSB) & MSR_DDSR_MASK)
#define MSR_DCTS_MSB                             0
#define MSR_DCTS_LSB                             0
#define MSR_DCTS_MASK                            0x00000001
#define MSR_DCTS_GET(x)                          (((x) & MSR_DCTS_MASK) >> MSR_DCTS_LSB)
#define MSR_DCTS_SET(x)                          (((x) << MSR_DCTS_LSB) & MSR_DCTS_MASK)


/************************** config definition ***************************/
#define UART_FIFO_SIZE	512 //Must be 2^N

#define USE_POST_BUFFER 0   // ENABLE a tx buffer for post processing,
/*********************** data struction definition ************************/
// data struction definition
struct uart_fifo {
#if USE_POST_BUFFER
	uint8_t buf[UART_FIFO_SIZE];
#endif
	uint16_t start_index;
	uint16_t end_index;
	uint32_t overrun_err;
};


struct uart_blk {
	uint16_t debug_mode;
	uint16_t baud;
	struct uart_api *_uart;
	struct uart_fifo _tx;
};

/******** hardware API table structure (API descriptions below) *************/
struct uart_api {
	uint32_t (*_uart_init)(void);
	void (*_uart_char_put)(uint8_t ch);
	uint16_t (*_uart_char_get)(uint8_t* ch);
	void (*_uart_str_out)(uint8_t* str);
	void (*_uart_task)(void);
	uint32_t (*_uart_status)(void);
	void (*_uart_config)(uint16_t flag);
	void (*_uart_hwinit)(uint32_t freq, uint32_t baud);
};

#endif	// end of _UART_API_H_

