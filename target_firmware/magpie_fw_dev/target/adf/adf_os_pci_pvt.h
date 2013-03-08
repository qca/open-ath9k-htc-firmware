#ifndef __ADF_OS_PCI_PVT_H
#define __ADF_OS_PCI_PVT_H

#include <wlan_pci.h>

//extern A_PCI_INIT_FUNC g_pci_init_func;

/**
 * init module macro
 */
#define __adf_os_pci_module_init(_fn)   A_PCI_INIT_FUNC g_pci_init_func = _fn;

/**
 * exit module macro
 */
#define __adf_os_pci_module_exit(_fn)   

/**
 * initiallize the PCI driver structure
 * Instance name will be <name>_pci_info
 */
#define __adf_os_pci_set_drv_info(_name, _pci_ids, _attach, _detach, _suspend, _resume)  \
{   \
    (_attach),  \
    (_detach),  \
    (_suspend),    \
    (_resume),  \
    ADF_OS_BUS_TYPE_PCI,    \
    { (_pci_ids)},   \
     #_name    \
};

/**
 * XXX: pci functions undone
 * @param osdev
 * @param offset
 * @param val
 * 
 * @return int
 */
static inline int 
__adf_os_pci_config_read8(adf_os_device_t osdev, int offset, a_uint8_t *val)
{
    (*val) = wlan_pci_config_read(offset, 1);
	return 0;
    /**
     * XXX:how do we know the read succeded
     */
}

static inline int 
__adf_os_pci_config_write8(adf_os_device_t osdev, int offset, a_uint8_t val)
{
    wlan_pci_config_write(offset, val, 1);
    return 0;    
}

static inline int 
__adf_os_pci_config_read16(adf_os_device_t osdev, int offset, a_uint16_t *val)
{
    (*val) = wlan_pci_config_read(offset, 2);
    return 0;
}

static inline int 
__adf_os_pci_config_write16(adf_os_device_t osdev, int offset, a_uint16_t val)
{
    wlan_pci_config_write(offset, val, 2);
    return 0;  
}

static inline int 
__adf_os_pci_config_read32(adf_os_device_t osdev, int offset, a_uint32_t *val)
{
    (*val) = wlan_pci_config_read(offset, 4);
    return 0;
}

static inline int 
__adf_os_pci_config_write32(adf_os_device_t osdev, int offset, a_uint32_t val)
{
    wlan_pci_config_write(offset, val, 4);
    return 0;  
}

#endif
