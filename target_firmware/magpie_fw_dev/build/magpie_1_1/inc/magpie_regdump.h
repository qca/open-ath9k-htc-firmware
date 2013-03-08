/*
 * Copyright (c) 2006 Atheros Communications Inc.
 * All rights reserved.
 */

#ifndef __MAGPIE_REGDUMP_H__
#define __MAGPIE_REGDUMP_H__

#if !defined(__ASSEMBLER__)
/*
 * XTensa CPU state
 * This must match the state saved by the target exception handler.
 */
struct XTensa_exception_frame_s {
    uint32_t xt_pc;
    uint32_t xt_ps;
    uint32_t xt_sar;
    uint32_t xt_vpri;
    uint32_t xt_a2;
    uint32_t xt_a3;
    uint32_t xt_a4;
    uint32_t xt_a5;
    uint32_t xt_exccause;
    uint32_t xt_lcount;
    uint32_t xt_lbeg;
    uint32_t xt_lend;

    /* Extra info to simplify post-mortem stack walkback */
#define MAGPIE_REGDUMP_FRAMES 5
    struct {
        uint32_t a0;  /* pc */
        uint32_t a1;  /* sp */
        uint32_t a2;
        uint32_t a3;
    } wb[MAGPIE_REGDUMP_FRAMES];
};

typedef struct XTensa_exception_frame_s CPU_exception_frame_t; 
#define RD_SIZE sizeof(CPU_exception_frame_t)

#endif
#endif /* __MAGPIE_REGDUMP_H__ */
