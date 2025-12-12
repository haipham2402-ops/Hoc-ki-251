#ifndef COMMON_H
#define COMMON_H

/* Define structs and functions used everywhere */
#include <stdint.h>
#include "os-cfg.h"
#include "os-mm.h" 

/* [FIX] Bỏ dòng #define ADDR_TYPE addr_t vì os-mm.h đã định nghĩa rồi */

enum ins_opcode_t {
    CALC,
    ALLOC,
    FREE,
    READ,
    WRITE,
    SYSCALL
};

struct code_seg_t {
    struct inst_t * text;
    uint32_t size;
};

struct inst_t {
    uint32_t opcode;
    uint32_t arg_0;
    uint32_t arg_1;
    uint32_t arg_2;
    uint32_t arg_3;
};

/* Forward declaration */
struct mm_struct;

struct pcb_t {
    uint32_t pid;
    uint32_t priority;
    char path[100];
    struct code_seg_t *code;
    addr_t regs[10];
    uint32_t pc;
#ifdef MLQ_SCHED
    uint32_t prio;
#endif
    struct page_table_t *page_table;
    uint32_t bp;
    struct krnl_t *krnl;
    struct mm_struct *mm;
};

struct krnl_t {
    void *ready_queue;
    void *running_list;
    void *mlq_ready_queue;
    struct memphy_struct *mram;
    struct memphy_struct **mswp;
    struct memphy_struct *active_mswp;
    struct mm_struct *mm;
};

#endif
