#ifndef __ADF_OS_IRQ_H
#define __ADF_OS_IRQ_H

#include <adf_os_types.h>
#include <adf_os_irq_pvt.h>

static inline int
adf_os_setup_intr(adf_os_device_t  osdev, adf_os_drv_intr fn)
{
    return __adf_os_setup_intr(osdev, fn);
}

static inline void
adf_os_free_intr(adf_os_device_t osdev)
{
    __adf_os_free_intr(osdev);
}

#endif
