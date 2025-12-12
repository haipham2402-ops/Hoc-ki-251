#ifndef SCHED_H
#define SCHED_H

/* Forward declaration để tránh lỗi vòng lặp include */
struct pcb_t;

#ifndef MLQ_SCHED
#define MLQ_SCHED
#endif

void init_scheduler(void);
struct pcb_t * get_proc(void);
void put_proc(struct pcb_t * proc);
void add_proc(struct pcb_t * proc);

#endif
