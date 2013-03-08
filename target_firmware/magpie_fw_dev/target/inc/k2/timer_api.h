#ifndef __TIMER_API_H__
#define __TIMER_API_H__
/*
 * In order to remain completely independent of OS header files,
 * "_SPACE" structures are declared with sufficient room to hold
 * corresponding OS structures.
 */
typedef unsigned int _A_TIMER_SPACE[5];
typedef _A_TIMER_SPACE A_timer_t;
#define A_TIMER A_timer_t /* historical */

typedef unsigned int A_HANDLE; /* historical */
typedef void A_TIMER_FUNC(A_HANDLE timer_handle, void *arg);

struct timer_api {
	void (* _timer_init)(void);
	void (* _timer_arm)(A_timer_t *, unsigned int);
	void (* _timer_disarm)(A_timer_t *);
	void (* _timer_setfn)(A_timer_t *, A_TIMER_FUNC, void *);
	void (* _timer_run)(void);
};
#endif /* __TIMER_API_H__ */

