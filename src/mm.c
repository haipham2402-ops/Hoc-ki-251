/* src/mm.c - Proc->mm Fixed Version */
#include "mm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if !defined(MM64)

int init_pte(addr_t *pte, int pre, addr_t fpn, int drt, int swp, int swptyp, addr_t swpoff) {
  if (pre != 0) {
    if (swp == 0) { 
      if (fpn == 0) return -1;
      SETBIT(*pte, PAGING_PTE_PRESENT_MASK);
      CLRBIT(*pte, PAGING_PTE_SWAPPED_MASK);
      CLRBIT(*pte, PAGING_PTE_DIRTY_MASK);
      SETVAL(*pte, fpn, PAGING_PTE_FPN_MASK, PAGING_PTE_FPN_LOBIT);
    } else { 
      SETBIT(*pte, PAGING_PTE_PRESENT_MASK);
      SETBIT(*pte, PAGING_PTE_SWAPPED_MASK);
      CLRBIT(*pte, PAGING_PTE_DIRTY_MASK);
      SETVAL(*pte, swptyp, PAGING_PTE_SWPTYP_MASK, PAGING_PTE_SWPTYP_LOBIT);
      SETVAL(*pte, swpoff, PAGING_PTE_SWPOFF_MASK, PAGING_PTE_SWPOFF_LOBIT);
    }
  }
  return 0;
}

int pte_set_swap(struct pcb_t *caller, addr_t pgn, int swptyp, addr_t swpoff) {
  struct krnl_t *krnl = caller->krnl;
  addr_t *pte = &caller->mm->pgd[pgn]; /* FIX HERE */
  SETBIT(*pte, PAGING_PTE_PRESENT_MASK);
  SETBIT(*pte, PAGING_PTE_SWAPPED_MASK);
  SETVAL(*pte, swptyp, PAGING_PTE_SWPTYP_MASK, PAGING_PTE_SWPTYP_LOBIT);
  SETVAL(*pte, swpoff, PAGING_PTE_SWPOFF_MASK, PAGING_PTE_SWPOFF_LOBIT);
  return 0;
}

int pte_set_fpn(struct pcb_t *caller, addr_t pgn, addr_t fpn) {
  struct krnl_t *krnl = caller->krnl;
  addr_t *pte = &caller->mm->pgd[pgn]; /* FIX HERE */
  SETBIT(*pte, PAGING_PTE_PRESENT_MASK);
  CLRBIT(*pte, PAGING_PTE_SWAPPED_MASK);
  SETVAL(*pte, fpn, PAGING_PTE_FPN_MASK, PAGING_PTE_FPN_LOBIT);
  return 0;
}

int init_mm(struct mm_struct *mm, struct pcb_t *caller) {
  struct vm_area_struct * vma = malloc(sizeof(struct vm_area_struct));
  mm->pgd = malloc(PAGING_MAX_PGN * sizeof(addr_t));
  for (int i = 0; i < PAGING_MAX_PGN; i++) mm->pgd[i] = 0;
  vma->vm_id = 0;
  vma->vm_start = 0;
  vma->vm_end = vma->vm_start;
  vma->sbrk = vma->vm_start;
  vma->vm_freerg_list = NULL;
  vma->vm_next = NULL; 
  mm->mmap = vma;
  mm->fifo_pgn = NULL; 
  memset(mm->symrgtbl, 0, sizeof(mm->symrgtbl));
  return 0;
}

struct vm_rg_struct *init_vm_rg(addr_t rg_start, addr_t rg_end) {
  struct vm_rg_struct *rgnode = malloc(sizeof(struct vm_rg_struct));
  rgnode->rg_start = rg_start;
  rgnode->rg_end = rg_end;
  rgnode->rg_next = NULL;
  return rgnode;
}

int enlist_vm_rg_node(struct vm_rg_struct **rglist, struct vm_rg_struct *rgnode) {
  rgnode->rg_next = *rglist;
  *rglist = rgnode;
  return 0;
}

int enlist_pgn_node(struct pgn_t **plist, addr_t pgn) {
  struct pgn_t *pnode = malloc(sizeof(struct pgn_t));
  pnode->pgn = pgn;
  pnode->pg_next = *plist;
  *plist = pnode;
  return 0;
}

int print_list_fp(struct framephy_struct *ifp) {
  struct framephy_struct *fp = ifp;
  printf("print_list_fp: ");
  if (fp == NULL) {printf("NULL list\n"); return -1;}
  printf("\n");
  while (fp != NULL ) {
     printf("fp[%d]\n",fp->fpn);
     fp = fp->fp_next;
  }
  printf("\n");
  return 0;
}

int print_list_rg(struct vm_rg_struct *irg) {
  struct vm_rg_struct *rg = irg;
  printf("print_list_rg: ");
  if (rg == NULL) {printf("NULL list\n"); return -1;}
  printf("\n");
  while (rg != NULL) {
     printf("rg[%08x->%08x]\n", rg->rg_start, rg->rg_end);
     rg = rg->rg_next;
  }
  printf("\n");
  return 0;
}

int print_list_vma(struct vm_area_struct *ivma) {
  struct vm_area_struct *vma = ivma;
  printf("print_list_vma: ");
  if (vma == NULL) {printf("NULL list\n"); return -1;}
  printf("\n");
  while (vma != NULL ) {
     printf("va[%08x->%08x]\n", vma->vm_start, vma->vm_end);
     vma = vma->vm_next;
  }
  printf("\n");
  return 0;
}

int print_list_pgn(struct pgn_t *ip) {
  printf("print_list_pgn: ");
  if (ip == NULL) {printf("NULL list\n"); return -1;}
  printf("\n");
  while (ip != NULL ) {
     printf("va[%08x]-\n", ip->pgn);
     ip = ip->pg_next;
  }
  printf("n");
  return 0;
}

int print_pgtbl(struct pcb_t *caller, uint32_t start, uint32_t end) {
  int pgn_start, pgn_end;
  int pgit;
  if(end == -1){
    pgn_start = 0;
    struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, 0); /* FIX HERE */
    end = cur_vma->vm_end;
  }
  pgn_start = PAGING_PGN(start);
  pgn_end = PAGING_PGN(end);
  printf("print_pgtbl: %d - %d", start, end);
  if (caller == NULL) {printf("NULL caller\n"); return -1;}
  printf("\n");
  for(pgit = pgn_start; pgit < pgn_end; pgit++) {
     uint32_t pte = caller->mm->pgd[pgit]; /* FIX HERE */
     if ((pte & PAGING_PTE_PRESENT_MASK) || (pte & PAGING_PTE_SWAPPED_MASK)) {
        printf("%08ld: %08x\n", pgit * sizeof(uint32_t), pte);
     }
  }
  return 0;
}

int vmap_pgd_memset(struct pcb_t *caller, addr_t addr, int pgnum) {
    return 0;
}

#endif
