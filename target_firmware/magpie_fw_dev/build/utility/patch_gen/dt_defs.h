/*************************************************************************/
/*  Copyright (c) 2006 Atheros Communications, Inc., All Rights Reserved */
/*                                                                       */
/*  Module Name : dt_defs.h                                              */
/*                                                                       */
/*  Abstract                                                             */
/*      This file contains the common data structure definition.	     */
/*                                                                       */
/*  NOTES                                                                */
/*      None                                                             */
/*                                                                       */
/*************************************************************************/

#ifndef _DT_DEFS_H
#define _DT_DEFS_H

#if 0
#define LOCAL	static
#else
#define LOCAL
#endif

/* data type definition */
typedef unsigned long	uint32_t;
typedef unsigned short	uint16_t;
typedef unsigned char	uint8_t;

typedef signed long		int32_t;
typedef signed short	int16_t;
typedef signed char		int8_t;

typedef uint16_t		BOOLEAN;

#define ptrData uint8_t *

/* marco definition */
#define SIZE_HASH_BUFFER       128

#define TRUE    (0==0)
#define FALSE   (0!=0)


#ifndef NULL
#define NULL	0x0
#endif

#endif
