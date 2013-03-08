#include "adf_os_defer_pvt.h"

void 
__adf_os_defer_func(void *arg, int pending)
{
	__adf_os_defer_ctx_t *ctx = (__adf_os_defer_ctx_t *)arg;

	ctx->caller_fn(ctx->caller_arg);
}


