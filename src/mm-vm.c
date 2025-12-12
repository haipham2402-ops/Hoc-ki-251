/* src/mm-vm.c - Bounds Check Fixed Version */
#include "string.h"
#include "mm.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/* Macros */
#define PAGING_SWPTYP(pte) ((pte & PAGING_PTE_SWPTYP_MASK) >> PAGING_PTE_SWPTYP_LOBIT)
#define PAGING_SWPOFF(pte) ((pte & PAGING_PTE_SWPOFF_MASK) >> PAGING_PTE_SWPOFF_LOBIT)
#define PAGING_PAGE_SWAPPED(pte) ((pte & PAGING_PTE_SWAPPED_MASK) != 0)

static pthread_mutex_t mmvm_lock = PTHREAD_MUTEX_INITIALIZER;

int validate_overlap_vm_area(struct pcb_t *caller, int vmaid, addr_t vmastart, addr_t vmaend) {
  struct vm_area_struct *vma = caller->mm->mmap;
  if (vmastart >= vmaend) return -1;
  while (vma != NULL) {
    if (vma->vm_id != vmaid && !((vmaend <= vma->vm_start) || (vmastart >= vma->vm_end))) return -1;
    vma = vma->vm_next;
  }
  return 0;
}

struct vm_area_struct *get_vma_by_num(struct mm_struct *mm, int vmaid) {
  struct vm_area_struct *pvma = mm->mmap;
  if (mm->mmap == NULL) return NULL;
  while (pvma != NULL) {
    if (pvma->vm_id == vmaid) return pvma;
    pvma = pvma->vm_next;
  }
  return NULL;
}

struct vm_rg_struct *get_symrg_byid(struct mm_struct *mm, int rgid) {
  if (rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ) return NULL;
  return &mm->symrgtbl[rgid];
}

int __swap_cp_page(struct memphy_struct *mpsrc, addr_t srcfpn, struct memphy_struct *mpdst, addr_t dstfpn) {
  int cellidx;
  int addrsrc, addrdst;
  for (cellidx = 0; cellidx < PAGING_PAGESZ; cellidx++) {
    addrsrc = srcfpn * PAGING_PAGESZ + cellidx;
    addrdst = dstfpn * PAGING_PAGESZ + cellidx;
    BYTE data;
    MEMPHY_read(mpsrc, addrsrc, &data);
    MEMPHY_write(mpdst, addrdst, data);
  }
  return 0;
}

int __mm_swap_page(struct pcb_t *caller, int vicfpn, int swpfpn) {
    __swap_cp_page(caller->krnl->mram, vicfpn, caller->krnl->active_mswp, swpfpn);
    MEMPHY_put_freefp(caller->krnl->mram, vicfpn);
    return 0;
}

addr_t alloc_pages_range(struct pcb_t *caller, int incpgnum, struct framephy_struct **frm_lst) {
  int pgit;
  int fpn_int; 
  for (pgit = 0; pgit < incpgnum; pgit++) {
    if (MEMPHY_get_freefp(caller->krnl->mram, &fpn_int) == 0) {
       struct framephy_struct *newnode = malloc(sizeof(struct framephy_struct));
       newnode->fpn = (addr_t)fpn_int;
       newnode->fp_next = *frm_lst;
       *frm_lst = newnode;
    } else { return -3000; }
  }
  return 0;
}

addr_t vmap_page_range(struct pcb_t *caller, addr_t addr, int pgnum, struct framephy_struct *frames, struct vm_rg_struct *ret_rg) {
  struct framephy_struct *fpit = frames;
  int pgn = PAGING_PGN(addr);
  int pgit = 0;
  ret_rg->rg_start = addr;
  ret_rg->rg_end = addr + pgnum * PAGING_PAGESZ;
  for (pgit = 0; pgit < pgnum; pgit++) {
     if (fpit == NULL) return -1;
     pte_set_fpn(caller, pgn + pgit, fpit->fpn);
     enlist_pgn_node(&caller->mm->fifo_pgn, pgn + pgit);
     fpit = fpit->fp_next;
  }
  return 0;
}

addr_t vm_map_ram(struct pcb_t *caller, addr_t astart, addr_t aend, addr_t mapstart, int incpgnum, struct vm_rg_struct *ret_rg) {
  struct framephy_struct *frames = NULL;
  addr_t ret_alloc = alloc_pages_range(caller, incpgnum, &frames);
  if ((int)ret_alloc == -3000) return -1;
  vmap_page_range(caller, mapstart, incpgnum, frames, ret_rg);
  return 0;
}

int inc_vma_limit(struct pcb_t *caller, int vmaid, addr_t inc_sz) {
  struct vm_rg_struct *newrg = malloc(sizeof(struct vm_rg_struct));
  int inc_amt = PAGING_PAGE_ALIGNSZ(inc_sz);
  int incnumpage =  inc_amt / PAGING_PAGESZ;
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  int old_end = cur_vma->sbrk;
  if (validate_overlap_vm_area(caller, vmaid, old_end, old_end + inc_amt) < 0) return -1; 
  if (vm_map_ram(caller, old_end, old_end + inc_amt, old_end, incnumpage , newrg) < 0) return -1; 
  cur_vma->sbrk += inc_amt;
  cur_vma->vm_end += inc_amt;
  enlist_vm_rg_node(&cur_vma->vm_freerg_list, newrg);
  return 0;
}

int find_victim_page(struct mm_struct *mm, addr_t *retpgn) {
  struct pgn_t *pg = mm->fifo_pgn;
  if (!pg) return -1;
  struct pgn_t *prev = NULL;
  while (pg->pg_next) { prev = pg; pg = pg->pg_next; }
  *retpgn = pg->pgn;
  if (prev) prev->pg_next = NULL; else mm->fifo_pgn = NULL;
  free(pg);
  return 0;
}

int get_free_vmrg_area(struct pcb_t *caller, int vmaid, int size, struct vm_rg_struct *newrg) {
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  struct vm_rg_struct *rgit = cur_vma->vm_freerg_list;
  if (rgit == NULL) return -1;
  while (rgit != NULL) {
    if (rgit->rg_start + size <= rgit->rg_end) { 
      newrg->rg_start = rgit->rg_start;
      newrg->rg_end = rgit->rg_start + size;
      rgit->rg_start += size;
      return 0;
    }
    rgit = rgit->rg_next;
  }
  return -1;
}

int __alloc(struct pcb_t *caller, int vmaid, int rgid, addr_t size, addr_t *alloc_addr) {
  struct vm_rg_struct rgnode;
  if (get_free_vmrg_area(caller, vmaid, size, &rgnode) == 0) {
    caller->mm->symrgtbl[rgid].rg_start = rgnode.rg_start;
    caller->mm->symrgtbl[rgid].rg_end = rgnode.rg_end;
    *alloc_addr = rgnode.rg_start;
    return 0;
  }
  int inc_sz = PAGING_PAGE_ALIGNSZ(size);
  inc_vma_limit(caller, vmaid, inc_sz);
  if (get_free_vmrg_area(caller, vmaid, size, &rgnode) == 0) {
    caller->mm->symrgtbl[rgid].rg_start = rgnode.rg_start;
    caller->mm->symrgtbl[rgid].rg_end = rgnode.rg_end;
    *alloc_addr = rgnode.rg_start;
    return 0;
  }
  return -1;
}

int __free(struct pcb_t *caller, int vmaid, int rgid) {
  if (rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ) return -1;
  struct vm_rg_struct *rgnode = get_symrg_byid(caller->mm, rgid);
  if (rgnode->rg_start == 0 && rgnode->rg_end == 0) return -1;
  struct vm_rg_struct *freerg_node = malloc(sizeof(struct vm_rg_struct));
  freerg_node->rg_start = rgnode->rg_start;
  freerg_node->rg_end = rgnode->rg_end;
  freerg_node->rg_next = NULL;
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  if (cur_vma) enlist_vm_rg_node(&cur_vma->vm_freerg_list, freerg_node);
  rgnode->rg_start = 0;
  rgnode->rg_end = 0;
  return 0;
}

int pg_getpage(struct mm_struct *mm, int pgn, int *fpn, struct pcb_t *caller) {
  if (pgn >= PAGING_MAX_PGN) return -1;

  uint32_t pte = mm->pgd[pgn];
  if (!PAGING_PAGE_PRESENT(pte)) {
    int vicpgn;
    uint32_t vicpte;
    int tgtfpn;
    if (MEMPHY_get_freefp(caller->krnl->mram, &tgtfpn) < 0) {
        addr_t vicpgn_addr;
        if (find_victim_page(caller->mm, &vicpgn_addr) < 0) return -1;
        vicpgn = (int)vicpgn_addr;
        vicpte = mm->pgd[vicpgn];
        int vicfpn = PAGING_FPN(vicpte);
        int swpfpn;
        MEMPHY_get_freefp(caller->krnl->active_mswp, &swpfpn);
        __swap_cp_page(caller->krnl->mram, vicfpn, caller->krnl->active_mswp, swpfpn);
        pte_set_swap(caller, vicpgn, 0, swpfpn); 
        tgtfpn = vicfpn;
    }
    if (PAGING_PAGE_SWAPPED(pte)) {
        int swpoff = PAGING_SWPOFF(pte);
        __swap_cp_page(caller->krnl->active_mswp, swpoff, caller->krnl->mram, tgtfpn);
        MEMPHY_put_freefp(caller->krnl->active_mswp, swpoff);
    }
    pte_set_fpn(caller, pgn, tgtfpn);
    enlist_pgn_node(&caller->mm->fifo_pgn, pgn);
  }
  *fpn = PAGING_FPN(mm->pgd[pgn]);
  return 0;
}

int pg_getval(struct mm_struct *mm, int addr, BYTE *data, struct pcb_t *caller) {
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;
  if (pg_getpage(mm, pgn, &fpn, caller) != 0) return -1;
  int phyaddr = (fpn * PAGING_PAGESZ) + off;
  MEMPHY_read(caller->krnl->mram, phyaddr, data);
  return 0;
}

int pg_setval(struct mm_struct *mm, int addr, BYTE value, struct pcb_t *caller) {
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;
  if (pg_getpage(mm, pgn, &fpn, caller) != 0) return -1;
  int phyaddr = (fpn * PAGING_PAGESZ) + off;
  MEMPHY_write(caller->krnl->mram, phyaddr, value);
  return 0;
}

int __read(struct pcb_t *caller, int vmaid, int rgid, addr_t offset, BYTE *data) {
  struct vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  if (currg == NULL || cur_vma == NULL) return -1;
  pg_getval(caller->mm, currg->rg_start + offset, data, caller);
  return 0;
}

int __write(struct pcb_t *caller, int vmaid, int rgid, addr_t offset, BYTE value) {
  struct vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  if (currg == NULL || cur_vma == NULL) return -1;
  pg_setval(caller->mm, currg->rg_start + offset, value, caller);
  return 0;
}

int liballoc(struct pcb_t *proc, uint32_t size, uint32_t reg_index) {
  addr_t addr;
  pthread_mutex_lock(&mmvm_lock);
  int val = __alloc(proc, 0, reg_index, size, &addr);
  pthread_mutex_unlock(&mmvm_lock);
  return val;
}

int libfree(struct pcb_t *proc, uint32_t reg_index) {
  pthread_mutex_lock(&mmvm_lock);
  int val = __free(proc, 0, reg_index);
  pthread_mutex_unlock(&mmvm_lock);
  return val;
}

int libread(struct pcb_t *proc, uint32_t source, uint32_t offset, uint32_t *destination) {
  BYTE data;
  pthread_mutex_lock(&mmvm_lock);
  int val = __read(proc, 0, source, offset, &data);
  pthread_mutex_unlock(&mmvm_lock);
  *destination = (uint32_t)data;
  return val;
}

int libwrite(struct pcb_t *proc, BYTE data, uint32_t destination, uint32_t offset) {
  pthread_mutex_lock(&mmvm_lock);
  int val = __write(proc, 0, destination, offset, data);
  pthread_mutex_unlock(&mmvm_lock);
  return val;
}
