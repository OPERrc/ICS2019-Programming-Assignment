#include "memory.h"
#include "proc.h"

static void *pf = NULL;


/*void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}*/

static void *start = NULL;
static bool pg_alloc[0x8000000 / PGSIZE] = {};
#define INDEX(p) (uint32_t)p / PGSIZE
void* new_page(size_t nr_page) {
  void *p = pf;
  pg_alloc[INDEX(p)] = true;
  pf += PGSIZE * nr_page;
  //int i = 0;
  while (pg_alloc[INDEX(pf)]) {
    pf += PGSIZE;
    if (pf >= (void *)_heap.end)
      pf = start;
    assert(pf == p);
  }
  //printf("_heap.end = 0x%x\n", _heap.end);
  //assert(0);
  //assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  if (p < start) 
    return;
    
  pg_alloc[INDEX(p)] = false;
  uint32_t *pte = (uint32_t *)p;
  for (int i = 0; i < PGSIZE / 4; i++)
    pte[i] = 0x00000000;
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  if ((brk & 0xfff) + increment < PGSIZE) 
    return 0;
  void *v_brk = (void *)((brk + PGSIZE) & ~0xfff);
  while ((uintptr_t)v_brk <= brk + increment) {
    //printf("v_brk = 0x%x\n", v_brk);
    void *p_mem = new_page(1);
    _map(&current->as, v_brk, p_mem, 0);
    v_brk += PGSIZE;
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  start = pf;
  for (int i = INDEX(start); i < INDEX(0x8000000); i++)
    pg_alloc[i] = false;
  Log("free physical pages starting from %p", pf);
  
  _vme_init(new_page, free_page);
}
