#ifndef __TASKLET_API_H__
#define __TASKLET_API_H__

//typedef int A_tasklet_t;

typedef void (*A_TASKLET_FUNC)(void *arg);

#define A_TASKLET_STATE_DISABLE         0
#define A_TASKLET_STATE_SCHEDULED       1
#define A_TASKLET_STATE_RUNNING         2

struct _tasklet {
    A_TASKLET_FUNC func;
    void *arg;
    int  state;
    struct _tasklet *next;
};

typedef struct _tasklet A_tasklet_t;

struct tasklet_api {
    void (* _tasklet_init)(void);
    void (* _tasklet_init_task)(A_TASKLET_FUNC, void * arg, A_tasklet_t *);
    void (* _tasklet_disable)(A_tasklet_t *);
    void (* _tasklet_schedule)(A_tasklet_t *);
    void (* _tasklet_run)(void);
};
#endif /* __TASKLET_API_H__ */
