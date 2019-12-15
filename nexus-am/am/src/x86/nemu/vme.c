#include <am.h>
#include <x86.h>
#include <nemu.h>
#include <klib.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN = {};
static PTE kptabs[(PMEM_SIZE + MMIO_SIZE) / PGSIZE] PG_ALIGN = {};
static void* (*pgalloc_usr)(size_t) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static _Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE},
  {.start = (void*)MMIO_BASE,  .end = (void*)(MMIO_BASE + MMIO_SIZE)}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;
  printf("kpdirs = 0x%x\n", kpdirs);
  printf("kptabs = 0x%x\n", kptabs);
  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;
      //printf("kpdirs[pdir_idx]? kpdirs[%d] = 0x%x\n", pdir_idx, kpdirs[pdir_idx]);

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      //printf("pte? = 0x%x\n", pte);
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      //printf("pte_end? = 0x%x\n", pte_end);
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        //printf("*ptab? = 0x%x\n", ptab);
        //printf("pte? = 0x%x\n", pte);
        ptab ++;
        //assert(0);
      }
    }
  }
  //printf("kpdirs = 0x%x\n", kpdirs);
  //assert(0);
  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);
  vme_enable = 1;
  
  return 0;
}

int _protect(_AddressSpace *as) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  as->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  return 0;
}

void _unprotect(_AddressSpace *as) {
}

static _AddressSpace *cur_as = NULL;
void __am_get_cur_as(_Context *c) {
  c->as = cur_as;
}

void __am_switch(_Context *c) {
  if (vme_enable) {
    set_cr3(c->as->ptr);
    cur_as = c->as;
  }
}

int _map(_AddressSpace *as, void *va, void *pa, int prot) {
  union {
    struct{
      uint32_t offset:12;
      uint32_t page:10;
      uint32_t dir:10;
    };
    uint32_t val;
  } v_addr;
  v_addr.val = (uint32_t)va;
  
  printf("va = 0x%x\n", v_addr.val);
  printf("pa = 0x%x\n", pa);
  printf("va.dir = 0x%x\n", v_addr.dir);
  printf("va.page = 0x%x\n", v_addr.page);
  printf("va.offset = 0x%x\n", v_addr.offset);
  PDE *updir = (PDE *)as->ptr;
  printf("updir = 0x%x\n", updir);
  //printf("updir[v_addr.dir] = 0x%x\n", updir[v_addr.dir]);
  if (updir[v_addr.dir] == 0)
    updir[v_addr.dir] = (uint32_t)(pgalloc_usr(1)) | 0x001;
  printf("updir[v_addr.dir] = 0x%x\n", updir[v_addr.dir]);
  PTE *uptabs = (PDE *)(updir[v_addr.dir]);
  printf("uptabs = 0x%x\n", uptabs);
  //printf("uptabs[v_addr.page] = 0x%x\n", uptabs[v_addr.page]);
  uptabs[v_addr.page] = (uint32_t)(pa - v_addr.offset)| 0x001;
  printf("uptabs[v_addr.page] = 0x%x\n", uptabs[v_addr.page]);
  //*(PDE *)(as->ptr + v_addr.dir * 4) = ;
  //*(PDE *)(as->ptr + v_addr.dir * 4) = ;
  //PTE *p_addr = (PTE *)(pa - v_addr.offset);
  //PDE *uptab_addr = (PTE *)(pa - v_addr.offset) - v_addr.page;
  //updir[v_addr.dir] = uptab_addr;
  //uptab_addr[v_addr.page] = p_addr;
  // assert(0);
  return 0;
}

_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack, void *entry, void *args) {
  _Context *new = ustack.end - 0x100 - sizeof(_Context);
  printf("as->ptr = 0x%x\n", as->ptr);
  new->eip = (uintptr_t)entry;
  new->cs = 8;
  new->as = as;
  void Context_display(_Context *c);
  Context_display(new);
  printf("--------------------------------\n");
  printf("in vme.c: _ucontext:\n");
  printf("_Context may have overlap bugs!\n");
  printf("--------------------------------\n");
  return new;
}
