/* src/mem.c - Dummy Version for Paging Mode */
#include "mem.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>

/* File này đã lỗi thời (Obsoleted) trong chế độ Paging.
 * Chúng ta giữ lại các hàm rỗng để không bị lỗi liên kết (Linker Error).
 */

void init_mem(void) {
    // Không làm gì cả
}

addr_t alloc_mem(uint32_t size, struct pcb_t * proc) {
    return 0;
}

int free_mem(addr_t address, struct pcb_t * proc) {
    return 0;
}

int read_mem(addr_t address, struct pcb_t * proc, BYTE * data) {
    return 0;
}

int write_mem(addr_t address, struct pcb_t * proc, BYTE data) {
    return 0;
}

void dump(void) {
    // Không làm gì cả
}
