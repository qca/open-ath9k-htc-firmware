#ifndef __USB_USER_PRE_H
#define __USB_USER_PRE_H

#define FUSB200_MAX_EP      10  // 1..10
#define FUSB200_MAX_FIFO    10  // 0.. 9
#define EP0MAXPACKETSIZE    0x40
// #define EP0FIFOSIZE          64  // EP0_FIFO
//JWEI 2003/04/29
//#define EP0MAXPACKETSIZE        0x08

// Max. Packet Size define
#define MX_PA_SZ_8          8
#define MX_PA_SZ_16         16
#define MX_PA_SZ_32         32
#define MX_PA_SZ_64         64
#define MX_PA_SZ_128        128
#define MX_PA_SZ_256        256
#define MX_PA_SZ_512        512
#define MX_PA_SZ_1024       1024

#define MASK_F0             0xF0

// Block Size define
#define BLK512BYTE      1
#define BLK1024BYTE     2

#define BLK64BYTE       1
#define BLK128BYTE      2

// Block toggle number define
#define SINGLE_BLK      1
#define DOUBLE_BLK      2
#define TRIBLE_BLK      3

// Endpoint transfer type
#define TF_TYPE_ISOCHRONOUS     1
#define TF_TYPE_BULK            2
#define TF_TYPE_INTERRUPT       3

// Endpoint or FIFO direction define
#define DIRECTION_IN    0
#define DIRECTION_OUT   1

// FIFO number define
#define FIFO0           0x0
#define FIFO1           0x1
#define FIFO2           0x2
#define FIFO3           0x3
#define FIFO4           0x4
#define FIFO5           0x5
#define FIFO6           0x6
#define FIFO7           0x7
#define FIFO8           0x8
#define FIFO9           0x9
#define FIFO10          10
#define FIFO11          11
#define FIFO12          12
#define FIFO13          13
#define FIFO14          14
#define FIFO15          15

// Descriptor Table uses the following parameters : fixed
#define DEVICE_LENGTH               0x12
#define CONFIG_LENGTH               0x09
#define INTERFACE_LENGTH            0x09
#define EP_LENGTH                   0x07
#define DEVICE_QUALIFIER_LENGTH     0x0A

//JWEI 2003/04/29
// Endpoint number define
#define EP0         0
#define EP1         1
#define EP2         2
#define EP3         3
#define EP4         4
#define EP5         5
#define EP6         6
#define EP7         7
#define EP8         8
#define EP9         9
#define EP10        10
#define EP11        11
#define EP12        12
#define EP13        13
#define EP14        14
#define EP15        15

#define STRING_00_LENGTH            0x04
#define STRING_10_LENGTH            0x0c
#define STRING_20_LENGTH            0x18
#define STRING_30_LENGTH            0x18
#define STRING_40_LENGTH            0x04
#define STRING_50_LENGTH            0x04
#define STRING_60_LENGTH            0x04
#define STRING_70_LENGTH            0x04
#define STRING_80_LENGTH            0x04
#define STRING_90_LENGTH            0x00

#endif
