#include <adf_os_types.h>
#include <adf_os_dma.h>
#include <adf_os_timer.h>
#include <adf_os_time.h>
#include <adf_os_lock.h>
#include <adf_os_io.h>
#include <adf_os_mem.h>
#include <adf_os_module.h>

#include <osapi.h>
#include <Magpie_api.h>

_A_magpie_indirection_table_t _indir_tbl;

int init_htc_tgt(void);

int init_htc_tgt(void)
{
	/* target-side HIF/HTC/WMI module installation */
	BUF_POOL_MODULE_INSTALL();
	HIF_MODULE_INSTALL();
	HTC_MODULE_INSTALL();
	WMI_SERVICE_MODULE_INSTALL();

	adf_os_print("HTC Target Version 1.xx Loaded...\n");
	return 0;
}

void exit_htc_tgt(void);

void exit_htc_tgt(void)
{
	adf_os_print("HTC Target UnLoaded...\n");
}

adf_os_export_symbol(_indir_tbl);

adf_os_virt_module_init(init_htc_tgt);
adf_os_virt_module_exit(exit_htc_tgt);
adf_os_module_dep(htc_tgt, adf_net);
adf_os_module_dep(htc_tgt, inproc_hif);
adf_os_virt_module_name(htc_tgt);
