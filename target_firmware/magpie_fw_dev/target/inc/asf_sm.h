/*
 * Copyright (c) 2007-2008 Atheros Communications, Inc.
 * All rights reserved.
 */
#ifndef _ASF_SM_H_
#define _ASF_SM_H_

typedef void (*asf_sm_fn_t)(void *object, int event);

typedef struct asf_sm_s {
    void *          object;
    asf_sm_fn_t     fn;
    void *          event_arg;
} asf_sm_t;

static inline void
asf_sm_init(asf_sm_t *sm, void *object)
{
    sm->object = object;
}

static inline void
asf_sm_set_state(asf_sm_t *sm, asf_sm_fn_t fn)
{
    sm->fn = fn;
}

static inline void
asf_sm_send_event(asf_sm_t *sm, int event)
{
    sm->fn(sm->object, event);
}

static inline void
asf_sm_set_event_arg(asf_sm_t *sm, void *arg)
{
    sm->event_arg = arg;
}

static inline void *
asf_sm_get_event_arg(asf_sm_t *sm)
{
    return sm->event_arg;
}

#endif /* _ASF_SM_H_ */
