
#include <adf_os_types.h>
#include <adf_os_util.h>

int     __adf_os_setup_intr(__adf_os_device_t  osdev, adf_os_drv_intr fn);
void    __adf_os_free_intr(__adf_os_device_t osdev);

extern adf_os_drv_intr         g_wlan_intr;

/**
 * @brief setup the Interrupt handler for the driver
 * @param[in] dev
 * @param[in] sc
 * 
 * @return int
 */

int
__adf_os_setup_intr(__adf_os_device_t  osdev, adf_os_drv_intr fn)
{
    g_wlan_intr = fn;
    
    return 0;    
}

/**
 * @brief deregister from the kernel the interrupt handler
 * @param[in] dev
 * @param[in] sc
 */
void
__adf_os_free_intr(__adf_os_device_t osdev)
{
    g_wlan_intr = NULL;  
}
