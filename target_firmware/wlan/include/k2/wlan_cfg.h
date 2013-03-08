#ifndef _WLAN_CFG_H_
#define _WLAN_CFG_H_

/************************** ATH configurations **************************/
#ifdef _DEBUG_BUILD_
#ifdef RX_SCATTER
#define ATH_RXDESC  30     /* number of RX descriptors */
#endif

#ifdef RX_SCATTER
#define ATH_RXBUF   ATH_RXDESC      /* number of RX buffers */
#else
#define ATH_RXBUF   15              /* number of RX buffers */
#endif
#define ATH_TXBUF   26              /* number of TX buffers */

#else

#ifdef RX_SCATTER
#define ATH_RXDESC  11     /* number of RX descriptors */
#endif   // end of _DEBUG_BUILD

#ifdef RX_SCATTER
#define ATH_RXBUF   ATH_RXDESC      /* number of RX buffers */
#else
#define ATH_RXBUF   15              /* number of RX buffers */
#endif
#define ATH_TXBUF   30              /* number of TX buffers */
#endif

#ifdef FUSION_USB_FW
#undef ATH_RXBUF
#undef ATH_TXBUF

#define ATH_RXBUF  11
#define ATH_TXBUF  33
#endif

#define ATH_BCBUF   1               /* number of beacon buffers */
#define ATH_WMI_MAX_CMD_REPLY   2
#define ATH_WMI_MAX_EVENTS      8

//#define ATH_DISABLE_RC              /* Use fixed rate instead of rate control */
#define ATH_BUF_OPTIMIZATION
#define ATH_NO_VIRTUAL_MEMORY
#define ATH_SUPPORT_XB_ONLY
#define ATH_ENABLE_WLAN_FOR_K2

#define ATH_VERSION_MAJOR 1
#define ATH_VERSION_MINOR 3
 
/************************** HAL configurations **************************/

#define HAL_DESC_OPTIMIZATION

#endif /* _WLAN_CFG_H_ */
