#include "dt_defs.h"

#define MAX_NUM_OF_PATCH	16

struct rom_patch_st {
#if 0
    uint16_t len;
    uint16_t crc16;
#else
    uint16_t crc16;
    uint16_t len;
#endif
    uint32_t ld_addr;
    uint32_t fun_addr;  // entry address of the patch code
    uint8_t *fun;
};
