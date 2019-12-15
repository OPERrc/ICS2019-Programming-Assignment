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
  /*printf("linear_addr = 0x%x\n", linear_addr.addr);
  printf("dir = 0x%x\n", linear_addr.dir);
  printf("page = 0x%x\n", linear_addr.page);
  printf("offset = 0x%x\n", linear_addr.offset);
  printf("cpu.cr3.page_directory_base = 0x%x\n", cpu.cr3.page_directory_base << 12);
  printf("dir = 0x%x\n", (cpu.cr3.page_directory_base << 12) + linear_addr.dir * 4);*/

  pde.val = paddr_read((cpu.cr3.page_directory_base << 12) + linear_addr.dir * 4, 4);
  //printf("pde = 0x%x\n", pde.val);
  assert(pde.present);

  pte.val = paddr_read((pde.page_frame << 12) + linear_addr.page * 4, 4);
  //printf("pte = 0x%x\n", pte.val);
  assert(pte.present);

  //panic("untested codes here!\n");
  //printf("paddr = 0x%x\n", (pte.page_frame << 12) + linear_addr.offset);
  //printf("paddr_read = 0x%x\n", paddr_read((pte.page_frame << 12) + linear_addr.offset, 4));
  return (pte.page_frame << 12) + linear_addr.offset;
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if ((addr & 0xfff) + len > PAGE_SIZE) {
    //printf("%d\n", cpu.cr0.paging);
    assert(0);
  }
  paddr_t paddr = (cpu.cr0.paging == 1 ? page_translate(addr) : addr);
  return paddr_read(paddr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if ((addr & 0xfff) + len > PAGE_SIZE) {
    //printf("%d\n", cpu.cr0.paging);
    assert(0);
  }
  paddr_t paddr = (cpu.cr0.paging == 1 ? page_translate(addr) : addr);
  paddr_write(paddr, data, len);
}
