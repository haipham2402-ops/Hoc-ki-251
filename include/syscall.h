#ifndef SYSCALL_H
#define SYSCALL_H

#include "common.h"

/* [FIX] Định nghĩa arg_t để tránh lỗi unknown type name */
typedef int arg_t;

struct sc_regs {
    arg_t a1;
    arg_t a2;
    arg_t a3;
    arg_t a4;
    arg_t a5;
    arg_t a6;
};

int syscall(struct krnl_t *krnl, uint32_t pid, uint32_t nr, struct sc_regs *regs);

/* Định nghĩa libsyscall */
int libsyscall(struct pcb_t *caller, uint32_t syscall_idx, arg_t a1, arg_t a2, arg_t a3);

#endif
