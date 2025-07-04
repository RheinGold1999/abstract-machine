#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      // printf("in vem_init for loop i = %d, va = %p\n", i, va);
      map(&kas, va, va, 0);
    }
  }

  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
  uint32_t *pdir = (uint32_t *)as->ptr;
  assert(pdir != NULL);

  uint32_t *ptb = NULL;
  size_t dir_i = ((uintptr_t)va >> 22) & 0x3FF;
  size_t tb_i = ((uintptr_t)va >> 12) & 0x3FF; 
  if ((pdir[dir_i] & 0x1) == 0) { // not present, allocate the page table
    ptb = (uint32_t *)pgalloc_usr(PGSIZE);
    pdir[dir_i] = (uintptr_t)ptb | 1;
  } else {  // present, get the page table
    ptb = (uint32_t *)(pdir[dir_i] & (~0xFFF));
  }
  // printf("dir_i = %d, tb_i = %d, pdir = %p, ptb = %p\n", dir_i, tb_i, pdir, ptb);
  ptb[tb_i] = ((uintptr_t)pa) & (~0xFFF);
  ptb[tb_i] |= 1;
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context *ctx = (Context *)(kstack.end - sizeof(Context));
  return ctx;
}
