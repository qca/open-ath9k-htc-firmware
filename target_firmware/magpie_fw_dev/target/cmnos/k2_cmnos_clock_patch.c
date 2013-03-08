#include "athos_api.h"
#include "sys_cfg.h"

a_uint32_t ref_clk = 0;
extern a_uint32_t cticks;

// clock change 
//
void cmnos_clock_init_patch(a_uint32_t refclk)
{
    ref_clk = refclk;
}

// retrieve current clock setting
// 
a_uint32_t cmnos_refclk_speed_get_patch(void)
{
    return ref_clk;
}


// software emulate delay function
//
void cmnos_delay_us_patch(int us)
{
    a_uint32_t start_time = NOW();
    unsigned int num_ticks = us*ref_clk; // system_freq == number of ticks per 1us
    
    while ( (NOW() - start_time) < num_ticks) {
        /* busy spin */
        ;
    }
}


// software emulate microsecond ticks
//
void cmnos_tick_patch(void)
{
    static a_uint32_t last_tick = 0;
    a_uint32_t current_tick = NOW();
    a_uint32_t delta_tick;

    delta_tick = (A_UINT32 ) (current_tick - last_tick)/(ref_clk<<10);

    if( delta_tick > 0 )
        last_tick = current_tick;

    cticks += delta_tick;
}

// get current sysmem up time in milliseconds based
// 
a_uint32_t cmnos_milliseconds_patch(void)
{
    cmnos_tick_patch();
    
    return (cticks);
}

