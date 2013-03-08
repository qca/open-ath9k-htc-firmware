/**
 * Copyright (c) 2002-2008 Atheros Communications, Inc.
 * All rights reserved
 * 
 * @module_name     ADF_NET
 * @module_desc     Magpie Network Shim
 */

#include <adf_net.h>
//#include <asf_queue.h>
//#include "adf_net_wcmd_pvt.h"
#include <wlan_pci.h>

/**
 * Prototypes
 */

/**
 * @brief register the driver into the shim
 * @param[in] drv
 * 
 * @return a_status_t
 */
a_status_t
__adf_net_register_drv(adf_drv_info_t *drv)
{
    wlan_pci_register_drv(drv);    
    return A_STATUS_OK;
}

/**
 * @brief unregister the driver from the shim
 * @param[in] name
 */
void
__adf_net_unregister_drv(a_uint8_t *name)
{
    // do nothing...
}
