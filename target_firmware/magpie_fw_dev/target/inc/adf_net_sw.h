/**
 * @ingroup adf_net_public
 * @file adf_net_sw.h
 * This file defines the device and virtual device switch tables.
 */ 

#ifndef __ADF_NET_SW_H
#define __ADF_NET_SW_H


/**
 * @brief per device switch structure
 */
typedef struct _adf_dev_sw{
    /**
     * @brief Handler for device open - mandatory interface
     */
    a_status_t        (*drv_open)      (adf_drv_handle_t hdl); 
    /**
     * @brief Handler for device close - mandatory interface
     */
    void              (*drv_close)     (adf_drv_handle_t hdl);
    /**
     * @brief Handler for transmit - mandatory interface
     */
    a_status_t        (*drv_tx)        (adf_drv_handle_t hdl, adf_nbuf_t pkt);
    /**
     * @brief Handler for configuration command - mandatory interface
     */
    a_status_t        (*drv_cmd)       (adf_drv_handle_t hdl, adf_net_cmd_t cmd,
                                        adf_net_cmd_data_t *data);
    /**
     * @brief Handler for ioctl - mandatory interface
     */
    a_status_t        (*drv_ioctl)     (adf_drv_handle_t hdl, int num, 
                                        void *data);
    /**
     * @brief Handler for transmission timeout - mandatory interface
     */
    a_status_t        (*drv_tx_timeout)(adf_drv_handle_t hdl);
    /**
     * @brief Handler for wireless configuration - optional interface
     */
    a_status_t  (*drv_wcmd) (adf_drv_handle_t hdl, adf_net_wcmd_type_t cmd,
                                           adf_net_wcmd_data_t *data);
    /** 
     * @brief Handler for polling if polling/deferred processing required - 
     * optional interface
     */
    adf_net_poll_resp_t (*drv_poll) (adf_drv_handle_t hdl, int quota, 
                                     int *work_done);
    /**
     * @brief Handler for per cpu deffered callback (e.g. for RSS) - optional
     * interface
     */
    adf_net_poll_resp_t (*drv_poll_cpu) (adf_drv_handle_t hdl, int quota, 
                                         int *work_done, void *arg);
    /**
     * @brief Handler for disabling receive interrupts for polling.
     * adf_drv should do proper locking - these are not called in atomic context
     */
    void (*drv_poll_int_disable)(adf_drv_handle_t hdl);
    /**
     * @brief Handler for enabling receive interrupts for polling.
     * adf_drv should do proper locking - these are not called in atomic context
     */
    void (*drv_poll_int_enable) (adf_drv_handle_t hdl);

}adf_dev_sw_t;

/**
 * @brief Virtual device switch structure
 */
typedef struct _adf_vdev_sw{
    /**
     * @brief Handler for device open
     */
    a_status_t  (*drv_open)     (adf_drv_handle_t hdl);
    /**
     * @brief Handler for device close
     */
    void        (*drv_close)    (adf_drv_handle_t hdl);
    /**
     * @brief Handler for transmit
     */
    a_status_t  (*drv_tx)   (adf_drv_handle_t hdl, adf_nbuf_t pkt);
    /**
     * @brief Handler for configuration command
     */
    a_status_t  (*drv_cmd)  (adf_drv_handle_t hdl, adf_net_cmd_t cmd,
                             adf_net_cmd_data_t *data);
    /**
     * @brief Handler for wireless configuration
     */
    a_status_t  (*drv_wcmd) (adf_drv_handle_t hdl, adf_net_wcmd_type_t cmd,
                             adf_net_wcmd_data_t *data);
    /**
     * @brief Handler for transmission timeout
     */
    a_status_t  (*drv_tx_timeout)   (adf_drv_handle_t hdl);
    /**
     * @brief Handler for ioctl
     */
    a_status_t  (*drv_ioctl)        (adf_drv_handle_t hdl, int num, void *data);
}adf_vdev_sw_t;

#endif

