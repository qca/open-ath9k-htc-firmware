/*
 * @File: wlan_pci.h
 * 
 * @Abstract: 
 * 
 * @Notes:
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef _WLAN_PCI_H
#define _WLAN_PCI_H

typedef int (*A_PCI_INIT_FUNC)(void);

//extern A_PCI_INIT_FUNC g_pci_init_func;

void wlan_pci_module_init(void);
void wlan_pci_register_drv(adf_drv_info_t *drv);
void wlan_pci_probe(void);
int  wlan_pci_config_write(int offset, a_uint32_t val, int width);
int  wlan_pci_config_read(int offset, int width);
void wlan_pci_isr();

#endif /* #ifndef _WLAN_PCI_H */
