#define _LITTLE_ENDIAN  1234    /* LSB first: i386, vax */
#define _BIG_ENDIAN 4321    /* MSB first: 68000, ibm, net */
#if defined(ADF_LITTLE_ENDIAN_MACHINE)
#define _BYTE_ORDER _LITTLE_ENDIAN
#elif defined(ADF_BIG_ENDIAN_MACHINE)
#define _BYTE_ORDER _BIG_ENDIAN
#else
#error "Please fix asm/byteorder.h"
#endif
