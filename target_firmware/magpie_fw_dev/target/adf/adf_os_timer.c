
#include <adf_os_types.h>
#include <adf_os_timer.h>

void
__adf_os_timer_func(A_HANDLE timer_handle, void *arg)
{
    __adf_os_timer_t *timer = (__adf_os_timer_t *)timer_handle;

	timer->timer_func(arg);
}
