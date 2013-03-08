/**
 * @ingroup adf_os_public
 * @file adf_os_pseudo.h
 * This file abstracts "pseudo module" semantics.
 */
#ifndef __ADF_OS_PSEUDO_H
#define __ADF_OS_PSEUDO_H

#include <adf_os_pseudo_pvt.h>

/**
 * @brief Specify the module's entry point.
 */ 
#define adf_os_pseudo_module_init(_fn)     __adf_os_pseudo_module_init(_fn)

/**
 * @brief Specify the module's exit point.
 */ 
#define adf_os_pseudo_module_exit(_fn)     __adf_os_pseudo_module_exit(_fn)

/**
 * @brief Setup the following driver information: name, pseudo IDs of devices
 * supported and some device handlers.
 */ 
#define adf_os_pseudo_set_drv_info(_name, _ifname, _pseudo_ids, _attach, _detach,  \
        _suspend, _resume) \
    __adf_os_pseudo_set_drv_info(_name, _ifname, _pseudo_ids, \
                                _attach, _detach, \
                                _suspend, _resume)
#endif

