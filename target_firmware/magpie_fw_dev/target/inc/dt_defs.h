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

#ifndef _DT_DEFS_H_
#define _DT_DEFS_H_

#ifndef LOCAL
#define LOCAL	static
#endif

/* data type definition */
typedef unsigned long	uint32_t;
typedef unsigned short	uint16_t;
typedef unsigned char	uint8_t;

typedef signed long     int32_t;
typedef signed short    int16_t;
typedef signed char     int8_t;

typedef uint16_t		BOOLEAN;

// Ray for porting
/* Basic data type */
#define u32_t   unsigned long
#define s32_t   signed long
#define u16_t   unsigned short
#define s16_t   signed short
#define u8_t    unsigned char
#define s8_t    signed char

#define ptrData uint8_t *

/* marco definition */
//#define SIZE_HASH_BUFFER       128

#ifndef TRUE
#define TRUE    (0==0)
#endif

#ifndef FALSE
#define FALSE   (0!=0)
#endif

#ifndef NULL
#define NULL	0x0
#endif

#define BIT0    (1<<0)
#define BIT1    (1<<1)
#define BIT2    (1<<2)
#define BIT3    (1<<3)
#define BIT4    (1<<4)
#define BIT5    (1<<5)
#define BIT6    (1<<6)
#define BIT7    (1<<7)
#define BIT8    (1<<8)
#define BIT9    (1<<9)
#define BIT10   (1<<10)
#define BIT11   (1<<11)
#define BIT12   (1<<12)
#define BIT13   (1<<13)
#define BIT14   (1<<14)
#define BIT15   (1<<15)
#define BIT16    (1<<16)
#define BIT17    (1<<17)
#define BIT18    (1<<18)
#define BIT19    (1<<19)
#define BIT20    (1<<20)
#define BIT21    (1<<21)
#define BIT22    (1<<22)
#define BIT23    (1<<23)
#define BIT24    (1<<24)
#define BIT25    (1<<25)
#define BIT26   (1<<26)
#define BIT27   (1<<27)
#define BIT28   (1<<28)
#define BIT29   (1<<29)
#define BIT30   (1<<30)
#define BIT31   (1<<31)

#endif

