/**
 * @ingroup adf_os_public
 * @file adf_os_module.h
 * This file abstracts "kernel module" semantics.
 */

#ifndef _ADF_OS_MODULE_H
#define _ADF_OS_MODULE_H

#include <adf_os_module_pvt.h>

typedef a_status_t (*module_init_func_t)(void);

/**
 * @brief Specify the module's entry point.
 */ 
#define adf_os_virt_module_init(_mod_init_func)  __adf_os_virt_module_init(_mod_init_func)

/**
 * @brief Specify the module's exit point.
 */ 
#define adf_os_virt_module_exit(_mod_exit_func)  __adf_os_virt_module_exit(_mod_exit_func)

/**
 * @brief Specify the module's name.
 */ 
#define adf_os_virt_module_name(_name)      __adf_os_virt_module_name(_name)

/**
 * @brief Specify the module's dependency on another module.
 */ 
#define adf_os_module_dep(_name,_dep)       __adf_os_module_dep(_name,_dep)

/**
 * @brief Export a symbol from a module.
 */ 
#define adf_os_export_symbol(_sym)         __adf_os_export_symbol(_sym)
     
/**
 * @brief Module parameter of type integer.
 */ 
#define ADF_OS_PARAM_TYPE_INT32             __ADF_OS_PARAM_TYPE_INT32

/**
 * @brief Module parameter of type string.
 */ 
#define ADF_OS_PARAM_TYPE_STRING            __ADF_OS_PARAM_TYPE_STRING

/**
 * @brief Declare a module parameter. 
 *
 * @param[in] name name of the parameter
 * @param[in] type type of the parameter
 *
 * @note These provide the config data defined by the userland
 * for this device. It can be queried at any time, given the name string
 * Only two types are supported
 * ADF_OS_PARAM_TYPE_STRING
 * ADF_OS_PARAM_TYPE_INT32
 * For example, say, the parameters name "my_int" and "my_name" are of 
 * variables of type int and string respectively. Then you would declare them 
 * as follows:
 * @code
 * adf_os_declare_param(my_int, ADF_OS_PARAM_TYPE_INT32);
 * adf_os_declare_param(my_name, ADF_OS_PARAM_TYPE_STRING);
 * @endcode
 * To read the userland provided config value, you would do something like
 *
 * @code
 * adf_os_read_param(my_name, &softc->sc_my_name);
 * @endcode
 *
 * or 
 * @code
 * st = adf_os_read_param(my_int, &softc->sc_my_int);
 * @endcode
 * st could be :
 *
 * A_STATUS_OK
 * A_STATUS_ENOMEM
 * A_STATUS_ENOENT
 *
 */
#define adf_os_declare_param(_name, _type) __adf_os_declare_param(_name, _type)

/**
 * @brief Read a parameter's value
 *
 * @param[in]  osdev    os handle
 * @param[in]  name     name of parameter
 * @param[in]  type     type of parameter
 * @param[out] val      value read
 *
 * @note pval is a point to the variable. Therefore,
 * for strings it is a_uint8_t **
 * for integers it is a_int_t *
 */
#define adf_os_read_param(_osdev, _name, _type, _pval)        \
                        __adf_os_read_param(_osdev, _name, _type, _pval)

#endif /*_ADF_OS_MODULE_H*/
