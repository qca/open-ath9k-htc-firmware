#ifndef __ADF_OS_MODULE_PVT_H
#define __ADF_OS_MODULE_PVT_H

//#include <sys/module.h>
//#include <sys/bus.h>


//int __adf_os_virt_mod(struct module *mod, int event, void *arg);




/**
 * MACRO's for initiallization
 */

/**
 * init module macro
 */
#define __adf_os_virt_module_init(_fn)

/**
 * @brief generic driver /module init function
 * 
 * @param mod (module data)
 * @param event (LOAD or UNLOAD)
 * @param arg (any extra argument needed if
 * 
 * @return int
 */
/**
 * exit module macro
 */
#define __adf_os_virt_module_exit(_fn)  

/**
 * initiallize a generic module
 */
#define __adf_os_virt_module_name(_name)    


#define __adf_os_module_dep(_name, _dep)    

#endif
