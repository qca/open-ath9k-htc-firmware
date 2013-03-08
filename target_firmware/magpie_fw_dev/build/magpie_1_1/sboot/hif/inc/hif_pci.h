
#ifndef __HIF_PCI_H
#define __HIF_PCI_H

#include <hif_api.h>


#define PCI_MAX_DATA_PKT_LEN            1600
#define PCI_MAX_CMD_PKT_LEN             64
#define PCI_MAX_BOOT_DESC               2 
     
typedef enum hif_pci_pipe_rx{
    HIF_PCI_PIPE_RX0, /*Normal Priority RX*/
    HIF_PCI_PIPE_RX1,
    HIF_PCI_PIPE_RX2,
    HIF_PCI_PIPE_RX3,
    HIF_PCI_PIPE_RX_MAX
}hif_pci_pipe_rx_t;

typedef enum hif_pci_pipe_tx{
    HIF_PCI_PIPE_TX0, /*Normal Priority TX*/
    HIF_PCI_PIPE_TX1,
    HIF_PCI_PIPE_TX_MAX
}hif_pci_pipe_tx_t;

struct pci_api{
    void (*pci_boot_init)(void);
};

void        cmnos_pci_module_install(struct pci_api *apis);
void        hif_pci_module_install(struct hif_api *apis);
#endif


