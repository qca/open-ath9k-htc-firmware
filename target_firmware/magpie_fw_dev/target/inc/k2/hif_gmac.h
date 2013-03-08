
#ifndef __HIF_GMAC_H
#define __HIF_GMAC_H

#include <adf_os_types.h>
#include <hif_api.h>


#define ETH_ALEN                6
#define GMAC_MAX_PKT_LEN        1600
#define GMAC_MAX_DESC           5

#define GMAC_DISCV_PKT_SZ       60
#define GMAC_DISCV_WAIT         2000

#define ATH_P_MAGBOOT           0x12 /*Magpie GMAC 18 for boot downloader*/
#define ATH_P_MAGNORM           0x13 /*Magpie GMAC 19 for HTC & others*/

#define ETH_P_ATH               0x88bd
     
typedef enum hif_gmac_pipe{
    HIF_GMAC_PIPE_RX = 1, /*Normal Priority RX*/
    HIF_GMAC_PIPE_TX = 2, /*Normal Priority TX*/
}hif_gmac_pipe_t;

struct gmac_api{
    void (*gmac_boot_init)(void);
};

void    cmnos_gmac_module_install(struct gmac_api *boot_apis);
void    hif_gmac_module_install(struct hif_api *apis);


#endif

