#ifndef __SFLASH_API_H__
#define __SFLASH_API_H__

/* Constant definition */
#define ZM_SFLASH_SECTOR_ERASE          1
#define ZM_SFLASH_BLOCK_ERASE           2
#define ZM_SFLASH_CHIP_ERASE            0

/*!- interface of eeprom access
 *
 */
struct sflash_api {
    /* Indispensable functions */
    void (* _sflash_init)(void);
    void (* _sflash_erase)(A_UINT32 erase_type, A_UINT32 addr);             /* 1. erase_type : chip/block/sector, 2. addr : no use for chip erase */
    void (* _sflash_program)(A_UINT32 addr, A_UINT32 len, A_UINT8 *buf);    /* 1. addr : spi flash address(start from 0x0), 2. len : bite number to write , 3. *buf : source memory address */
    void (* _sflash_read)(A_UINT32 fast, A_UINT32 addr, A_UINT32 len, A_UINT8 *buf);    /* 1. fast : 1 for fast read, 0 for read, 2. addr : spi flash address(start from 0x0), 3. len : bite number to read , 3. *buf : destination memory address */

    /* Dispensable functions */
    A_UINT32 (* _sflash_rdsr)(void); /* return the value of status register */
};

#endif /* __SFLASH_API_H__ */

