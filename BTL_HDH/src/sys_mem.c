/* src/sys_mem.c - Type Fixed Version */
#include "os-mm.h"
#include "syscall.h"
#include "libmem.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef MM64
#include "mm64.h"
#else
#include "mm.h"
#endif

// Khai báo hàm này nếu chưa có trong header để tránh warning
extern int vmap_pgd_memset(struct pcb_t *caller, addr_t addr, int pgnum);

int __sys_memmap(struct krnl_t *krnl, uint32_t pid, struct sc_regs* regs)
{
   int memop = regs->a1;

   struct pcb_t *caller = malloc(sizeof(struct pcb_t));
   caller->krnl = krnl;
   caller->pid = pid;

   switch (memop) {
   case SYSMEM_MAP_OP:
            // Ép kiểu tham số sang addr_t
            vmap_pgd_memset(caller, (addr_t)regs->a2, (int)regs->a3);
            break;
   case SYSMEM_INC_OP:
            inc_vma_limit(caller, (int)regs->a2, (addr_t)regs->a3);
            break;
   case SYSMEM_SWP_OP:
            __mm_swap_page(caller, (int)regs->a2, (int)regs->a3);
            break;
   case SYSMEM_IO_READ:
            {
                BYTE value;
                
                MEMPHY_read(caller->krnl->mram, (addr_t)regs->a2, &value);
                regs->a3 = (uint32_t)value; 
            }
            break;
   case SYSMEM_IO_WRITE:
            MEMPHY_write(caller->krnl->mram, (addr_t)regs->a2, (BYTE)regs->a3);
            break;
   default:
            printf("Memop code: %d\n", memop);
            break;
   }

   free(caller); 
   return 0;
}
