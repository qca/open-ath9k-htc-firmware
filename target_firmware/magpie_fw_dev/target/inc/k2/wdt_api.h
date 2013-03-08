#ifndef __WDT_API_H__
#define __WDT_API_H__

typedef enum {
	WDT_ACTION_NO = 0,      // bit1, bit0: 00
	WDT_ACTION_INTR,        // bit1, bit0: 01
	WDT_ACTION_NMI,         // bit1, bit0: 10
	WDT_ACTION_RESET,     // bit1, bit0: 11

	WDT_ACTION_UNKNOWN
} T_WDT_ACTION_TYPE;

typedef enum {
	WDT_TIMEOUT = 1,
	WDT_ACTION,

	WDT_UNKNOWN
} T_WDT_CMD_TYPE;

typedef struct {
	uint32_t cmd;
	union {
		uint32_t timeout;
		uint32_t action;
	};
}T_WDT_CMD;

typedef enum {
	ENUM_WDT_BOOT = 1,
	ENUM_COLD_BOOT,
	ENUM_SUSP_BOOT,

	// add above here
	ENUM_UNKNOWN_BOOT
} T_BOOT_TYPE;


/*!- interface of watchdog timer
 *
 */
struct wdt_api {
	void (* _wdt_init)(void);
	void (* _wdt_enable)(void);
	void (* _wdt_disable)(void);
	void (* _wdt_set)(T_WDT_CMD);
	void (* _wdt_task)(void);
	void (* _wdt_reset)(void);
	T_BOOT_TYPE (*_wdt_last_boot)(void);
};
#endif /* __WDT_API_H__ */

