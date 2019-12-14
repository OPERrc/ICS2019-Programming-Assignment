#include "nemu.h"
#include "isa/mmu.h"

paddr_t page_translate(vaddr_t addr) {
  union {
    struct{
      uint32_t offset:12;
      uint32_t page:10;
      uint32_t dir:10;
    };
    vaddr_t addr;
  } linear_addr;

  PDE pde;
  PTE pte;

  linear_addr.addr = addr;
  //printf("linear_addr = 0x%x\n", addr);

  pde.val = paddr_read(cpu.cr3.page_directory_base + linear_addr.dir * 4, 4);
  assert(pde.present == 0);
  //printf("pde = 0x%x\n", pde.val);

  pte.val = paddr_read(pde.page_frame + linear_addr.page * 4, 4);
  assert(pte.present == 0);
  //printf("pte = 0x%x\n", pte.val);

  //panic("untested codes here!\n");
  return paddr_read(pte.page_frame + linear_addr.offset * 4, 4);
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  /*if (cpu.cr0.paging != 0) {
    printf("%d\n", cpu.cr0.paging);
    assert(0);
  }*/
  paddr_t paddr = (cpu.cr0.paging == 1 ? page_translate(addr) : addr);
  return paddr_read(paddr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_t paddr = (cpu.cr0.paging == 1 ? page_translate(addr) : addr);
  paddr_write(paddr, data, len);
}
