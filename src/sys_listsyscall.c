/* src/sys_listsyscall.c - Fixed Version */
#include <stdio.h>
#include "syscall.h"

/* Khai báo extern để truy cập biến từ syscall.c */
extern const char* sys_call_table[];
extern const int syscall_table_size;

int __sys_listsyscall(struct krnl_t *krnl, uint32_t pid, struct sc_regs* reg)
{
   /* Duyệt và in danh sách system call */
   for (int i = 0; i < syscall_table_size; i++) {
       // Kiểm tra null để tránh lỗi
       if (sys_call_table[i]) {
           printf("%s\n", sys_call_table[i]); 
       }
   }
   return 0;
}
