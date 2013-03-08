#ifndef __ADF_OS_IRQ_PVT_H
#define __ADF_OS_IRQ_PVT_H

int     __adf_os_setup_intr(__adf_os_device_t  osdev, adf_os_drv_intr fn);
void    __adf_os_free_intr(__adf_os_device_t osdev);

#endif
