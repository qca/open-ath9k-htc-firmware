
#include "sys_cfg.h"

#include "athos_api.h"

#if SYSTEM_MODULE_WDT


typedef struct {
    BOOLEAN             state;
    T_WDT_ACTION_TYPE   action;
    uint32_t            timeout;
}T_WDT_CTRL;


LOCAL T_WDT_CTRL wdt_ctrl;

/*!- Initialize watchdog timer
 *
 */
LOCAL T_BOOT_TYPE
cmnos_wdt_last_boot(void)
{  
#if 0
    if (HAL_WORD_REG_READ(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR) & 0x80000000 )
        return ENUM_WDT_BOOT;
    else
        return ENUM_COLD_BOOT;
#endif

#if defined(PROJECT_MAGPIE)
    if ( *((volatile uint32_t*)WATCH_DOG_MAGIC_PATTERN_ADDR) == WDT_MAGIC_PATTERN )
        return ENUM_WDT_BOOT;
    else if ( *((volatile uint32_t*)WATCH_DOG_MAGIC_PATTERN_ADDR) == SUS_MAGIC_PATTERN )
        return ENUM_SUSP_BOOT;
    else
        return ENUM_COLD_BOOT;
#elif defined(PROJECT_K2)
    //if (HAL_WORD_REG_READ(MAGPIE_REG_RST_STATUS_ADDR) == WDT_MAGIC_PATTERN || HAL_WORD_REG_READ(MAGPIE_REG_RST_STATUS_ADDR) == SUS_MAGIC_PATTERN)
    if ( HAL_WORD_REG_READ(MAGPIE_REG_RST_STATUS_ADDR) == WDT_MAGIC_PATTERN )
        return ENUM_WDT_BOOT;
    else if ( HAL_WORD_REG_READ(MAGPIE_REG_RST_STATUS_ADDR) == SUS_MAGIC_PATTERN )
        return ENUM_SUSP_BOOT;
    else
        return ENUM_COLD_BOOT;
#endif /* #if defined(PROJECT_MAGPIE) */
}


/*!- trigger a wdt reset.
 *
 */
LOCAL void
cmnos_wdt_reset()
{
    HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR, WDT_ACTION_RESET);
    HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_ADDR, 0x100); 
    while(1);   // never return;   
}


/*!- setup the timeout value.
 *
 */
LOCAL void
cmnos_wdt_set(T_WDT_CMD wdt_cmd)
{
    switch(wdt_cmd.cmd)
    {
        case WDT_TIMEOUT:
            if( wdt_cmd.timeout>0 ) //dummy check
            {
                wdt_ctrl.timeout = wdt_cmd.timeout;
                HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_ADDR, wdt_ctrl.timeout); 
            }
            break;

        case WDT_ACTION:
            if( wdt_cmd.action < WDT_ACTION_UNKNOWN )
            {
                wdt_ctrl.action = wdt_cmd.action;
                HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR, wdt_ctrl.action);
            }
    }
}


/*!- Enable watchdog timer.
 *
 */
LOCAL void
cmnos_wdt_enable()
{
    T_WDT_CMD wdt_cmd;
    wdt_ctrl.state = TRUE;

    wdt_cmd.cmd = WDT_ACTION;
    wdt_cmd.action = WDT_ACTION_RESET;
    cmnos_wdt_set(wdt_cmd);
    //HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR, HAL_WORD_REG_READ(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR)|(BIT0|BIT1));
}

/*!- Disable watchdog timer.
 *
 */
LOCAL void
cmnos_wdt_disable()
{
    T_WDT_CMD wdt_cmd;

    wdt_ctrl.state = FALSE;

    wdt_cmd.cmd = WDT_ACTION;
    wdt_cmd.action = WDT_ACTION_NO;
    cmnos_wdt_set(wdt_cmd);
//    HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR, (HAL_WORD_REG_READ(MAGPIE_REG_RST_WDT_TIMER_CTRL_ADDR)&(~(BIT0|BIT1))));
//    MAGPIE_REG_RST_WDT_TIMER_CTRL &= ~(BIT0|BIT1);
}


/*!- Initialize watchdog timer
 *
 */
LOCAL void
cmnos_wdt_init(void)
{
    T_WDT_CMD wdt_cmd;
    
    //wdt_init_done = TRUE;
    wdt_ctrl.state = FALSE;
    wdt_ctrl.action = WDT_ACTION_NO;
    wdt_ctrl.timeout = WDT_DEFAULT_TIMEOUT_VALUE;

    // should be init outside but just kick it ticking at here for workaround
    cmnos_wdt_enable();

    wdt_cmd.cmd = WDT_TIMEOUT;
    wdt_cmd.timeout= wdt_ctrl.timeout;
    cmnos_wdt_set(wdt_cmd);
}


/*!- update the watchdog timer timerout value.
 *
 */
LOCAL void
cmnos_wdt_task()
{
    // if wdt is not enable, just skip the update
    if( wdt_ctrl.state )
    {
        HAL_WORD_REG_WRITE(MAGPIE_REG_RST_WDT_TIMER_ADDR, wdt_ctrl.timeout); 
    }
    
}

void
cmnos_wdt_module_install(struct wdt_api *tbl)
{
    tbl->_wdt_init          = cmnos_wdt_init;
    tbl->_wdt_enable        = cmnos_wdt_enable;
    tbl->_wdt_disable       = cmnos_wdt_disable;
    tbl->_wdt_set           = cmnos_wdt_set;
    tbl->_wdt_task          = cmnos_wdt_task;
    tbl->_wdt_reset         = cmnos_wdt_reset;
    tbl->_wdt_last_boot     = cmnos_wdt_last_boot;
}

#endif /* SYSTEM_MODULE_WDT */

