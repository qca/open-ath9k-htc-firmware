#ifndef __CLOCK_API_H__
#define __CLOCK_API_H__

#define TICK_MSEC_RATIO			1
#define TICK_TO_MSEC(tick)		((tick)/TICK_MSEC_RATIO)
#define MSEC_TO_TICK(msec)		((msec)* TICK_MSEC_RATIO)

typedef struct date_s {
	uint16_t miliseconds;
	uint16_t seconds;
	uint16_t minutes;
	uint16_t hours;
} A_DATE_T;

struct clock_api {
	void (* _clock_init)(A_UINT32 ref_clk);
	void (* _clockregs_init)(void);
	A_UINT32 (* _uart_frequency)(void);
	void (* _delay_us)(int);
	void (* _wlan_band_set)(int);
	A_UINT32 (* _refclk_speed_get)(void);
	A_UINT32 (* _milliseconds)(void);
	void (* _sysclk_change)(uint32_t sys_clk);
	A_UINT32 (* _sysclk_get)(void);

	void (* _clock_tick)(void);
};

#endif /* __CLOCK_API_H__ */
