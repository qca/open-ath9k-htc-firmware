#ifndef _WLAN_CFG_H_
#define _WLAN_CFG_H_

/************************** ATH configurations **************************/
#ifdef RX_SCATTER
#define ATH_RXDESC  40     /* number of RX descriptors */
#endif

#ifdef RX_SCATTER
#define ATH_RXBUF   40              /* number of RX buffers */
#else
#define ATH_RXBUF   20              /* number of RX buffers */
#endif
#define ATH_TXBUF   45              /* number of TX buffers */
#define ATH_BCBUF   1               /* number of beacon buffers */
#define ATH_WMI_MAX_CMD_REPLY   2
#define ATH_WMI_MAX_EVENTS      8

#ifdef FUSION_USB_FW
#undef ATH_RXBUF
#undef ATH_RXDESC

#define ATH_RXDESC  20
#define ATH_RXBUF   20
#endif
//#define ATH_DISABLE_RC              /* Use fixed rate instead of rate control */

#define ATH_BUF_OPTIMIZATION
#define ATH_NO_VIRTUAL_MEMORY

//#define ATH_SUPPORT_XB_ONLY
#define ATH_SUPPORT_A_MODE 

#define ATH_VERSION_MAJOR 1
#define ATH_VERSION_MINOR 3

/************************** HAL configurations **************************/

#define HAL_DESC_OPTIMIZATION

#endif /* _WLAN_CFG_H_ */
