# ifndef _PSTAT_H_
# define _PSTAT_H_
# include "param.h"

struct pstat {
    int inuse[NPROC]; // 1 if this slot is in use
    int pid[NPROC]; // PID
    int ticks[NPROC]; // # times scheduled (proxy for CPU time)
    int size[NPROC];
    // int wait_ticks[NPROC]; // total time spent RUNNABLE
    // int start_tick[NPROC]; // creation time (ticks)
    // int first_run[NPROC]; // first scheduled time (ticks), -1 if never
    // int end_tick[NPROC]; // completion time (ticks), else 0
    // char name[NPROC][16]; // process name
};
# endif
