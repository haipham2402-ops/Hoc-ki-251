/*
 * Copyright (C) 2026 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* LamiaAtrium release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t queue_lock;

static struct queue_t running_list;
#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
static int slot[MAX_PRIO];
#endif

int queue_empty(void) {
#ifdef MLQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
    int i ;
	for (i = 0; i < MAX_PRIO; i ++) {
		mlq_ready_queue[i].size = 0;
		slot[i] = MAX_PRIO - i; // Thiết lập slot theo công thức đề bài
	}
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	running_list.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED
/* * Stateful design for routine calling
 * based on the priority and our MLQ policy
 * We implement stateful here using transition technique
 * State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
struct pcb_t * get_mlq_proc(void) {
	struct pcb_t * proc = NULL;

	pthread_mutex_lock(&queue_lock);
	/*TODO: get a process from PRIORITY [ready_queue].
	 * It worth to protect by a mechanism.
	 * */
    
    /* Logic MLQ: Quét qua các hàng đợi để tìm tiến trình.
       Tuy nhiên, để đảm bảo công bằng theo slot, ta không nên luôn luôn bắt đầu từ 0.
       Nhưng trong khuôn khổ bài tập đơn giản, ta có thể dùng biến static
       để nhớ vị trí queue đang xét dở.
    */
    static int curr_prio = 0;

    // Duyệt tối đa một vòng qua các mức ưu tiên để tìm process
    for (int i = 0; i < MAX_PRIO; i++) {
        // Nếu queue tại curr_prio có người
        if (!empty(&mlq_ready_queue[curr_prio])) {
            
            // Nếu còn slot để chạy
            if (slot[curr_prio] > 0) {
                proc = dequeue(&mlq_ready_queue[curr_prio]);
                slot[curr_prio]--; // Trừ slot
                break; // Tìm thấy rồi thì thoát vòng lặp
            } 
            else {
                // Hết slot: Reset slot và chuyển sang queue kế tiếp
                slot[curr_prio] = MAX_PRIO - curr_prio;
                curr_prio = (curr_prio + 1) % MAX_PRIO;
                
                // Sau khi reset, nếu queue mới vẫn có process thì lấy luôn (hoặc continue để vòng lặp xử lý)
                // Ở đây ta continue để vòng lặp kiểm tra lại điều kiện slot > 0 ở bước sau
                if (!empty(&mlq_ready_queue[curr_prio])) {
                    // Để đơn giản, ta lùi i lại 1 bước để vòng lặp for xét lại ngay curr_prio mới này
                    i--; 
                    continue;
                }
            }
        } 
        else {
            // Queue rỗng: Reset slot (đề phòng) và chuyển tiếp
            slot[curr_prio] = MAX_PRIO - curr_prio;
            curr_prio = (curr_prio + 1) % MAX_PRIO;
        }
    }

	pthread_mutex_unlock(&queue_lock);

	if (proc != NULL) {
        // Cần lock khi truy cập running_list (shared resource)
        // Tuy nhiên hàm get_mlq_proc thường được gọi đơn lẻ, 
        // nhưng an toàn nhất là nên lock running_list nếu nó được dùng ở chỗ khác.
        // Ở đây ta giả định queue_lock bảo vệ cả running_list hoặc running_list chưa cần lock chặt.
		/* Cập nhật thông tin kernel cho proc (quan trọng cho phần MM sau này) */
        proc->krnl->ready_queue = &ready_queue;
        proc->krnl->mlq_ready_queue = mlq_ready_queue;
        proc->krnl->running_list = &running_list;
	}
		
	return proc;	
}

void put_mlq_proc(struct pcb_t * proc) {
	/* TODO: put running proc to running_list 
	 * It worth to protect by a mechanism.
	 * */
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(struct pcb_t * proc) {
	/* TODO: put running proc to running_list
	 * It worth to protect by a mechanism.
	 * */
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

struct pcb_t * get_proc(void) {
	return get_mlq_proc();
}

void put_proc(struct pcb_t * proc) {
	return put_mlq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
	return add_mlq_proc(proc);
}
#else
struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;

	pthread_mutex_lock(&queue_lock);
	/*TODO: get a process from [ready_queue].
	 * It worth to protect by a mechanism.
	 * */
    if (!empty(&ready_queue)) {
        proc = dequeue(&ready_queue);
    }
	pthread_mutex_unlock(&queue_lock);

	return proc;
}

void put_proc(struct pcb_t * proc) {
	/* TODO: put running proc to running_list 
	 * It worth to protect by a mechanism.
	 * */
	pthread_mutex_lock(&queue_lock);
	enqueue(&run_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t * proc) {
	/* TODO: put running proc to running_list 
	 * It worth to protect by a mechanism.
	 * */
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif