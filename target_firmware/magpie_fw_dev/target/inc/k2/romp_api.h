/*************************************************************************/
/*  Copyright (c) 2008 Atheros Communications, Inc., All Rights Reserved */
/*                                                                       */
/*  Module Name : romp_api.h     	                                 */
/*                                                                       */
/*  Abstract                                                             */
/*      This file contains definition of data structure and interface    */
/*                                                                       */
/*  NOTES                                                                */
/*      None                                                             */
/*                                                                       */
/*************************************************************************/

#ifndef _ROMP_API_H_
#define _ROMP_API_H_

#include "dt_defs.h"

/******** hardware API table structure (API descriptions below) *************/

struct romp_api {
	void (*_romp_init)(void);
	BOOLEAN (*_romp_download)(uint16_t );
	BOOLEAN (*_romp_install)(void);
	BOOLEAN (*_romp_decode)(uint32_t );
};

#define _ROMP_MAGIC_ "[PaTcH]"

struct rom_patch_st {
	uint16_t crc16;		// crc filed to maintain the integrity
	uint16_t len;		// length of the patch code
	uint32_t ld_addr;	// load address of the patch code
	uint32_t fun_addr;  // entry address of the patch code
	uint8_t *pfun;		// patch code
};


struct eep_redir_addr {
	uint16_t offset;
	uint16_t size;
};

/************************* EXPORT function ***************************/
uint16_t cal_crc16(uint32_t sz, uint8_t *p);

#endif	// end of _UART_API_H_

