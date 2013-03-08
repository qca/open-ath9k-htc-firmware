#ifndef __REGDUMP_H__
#define __REGDUMP_H__

#include "magpie_regdump.h"


#if !defined(__ASSEMBLER__)

/*
 * XTensa CPU state
 * This must match the state saved by the target exception handler.
 */
 
#define RD_SIZE sizeof(CPU_exception_frame_t)

/*
 * Target CPU state at the time of failure is reflected
 * in a register dump, which the Host can fetch through
 * the diagnostic window.
 */
struct register_dump_s {
	uint32_t target_id;               /* Target ID */
	uint32_t assline;                 /* Line number (if assertion failure) */
	uint32_t pc;                      /* Program Counter at time of exception */
	uint32_t badvaddr;                /* Virtual address causing exception */
	CPU_exception_frame_t exc_frame;  /* CPU-specific exception info */

	/* Could copy top of stack here, too.... */
};

#endif /* __ASSEMBLER__ */
#endif /* __REGDUMP_H__ */
