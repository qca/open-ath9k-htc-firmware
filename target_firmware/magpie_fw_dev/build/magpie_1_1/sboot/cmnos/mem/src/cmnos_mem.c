
#include "sys_cfg.h"

#if SYSTEM_MODULE_MEM

#include "athos_api.h"

LOCAL void
cmnos_mem_init(void)
{
}

/* Memory operations are provided by toolchain libraries. */

extern void *memset(void *dest, int val, unsigned int nbyte);
extern void *memcpy(void *dest, const void *src, unsigned int nbyte);
extern void *memmove(void *dest, const void *src, unsigned int nbyte);
extern int   memcmp(const void *string1, const void *string2, unsigned int nbyte);

void
cmnos_mem_module_install(struct mem_api *tbl)
{
    tbl->_mem_init      = cmnos_mem_init;
    tbl->_memset        = memset;
    tbl->_memcpy        = memcpy;
    tbl->_memmove       = memmove;
    tbl->_memcmp        = memcmp;
}
#endif

