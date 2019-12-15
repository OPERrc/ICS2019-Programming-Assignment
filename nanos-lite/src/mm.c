#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  if ((brk & 0xfff) + increment < PGSIZE) 
    return 0;
  void *v_brk = (void *)((brk + PGSIZE) & ~0xfff);
  while ((uintptr_t)v_brk <= brk + increment) {
    printf("v_brk = 0x%x\n", v_brk);
    void *p_mem = new_page(1);
    _map(&current->as, v_brk, p_mem, 0);
    v_brk += PGSIZE;
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
