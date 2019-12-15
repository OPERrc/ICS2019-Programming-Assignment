#include "nemu.h"
#include "isa/mmu.h"

#define print_debug_info \
  printf("linear_addr = 0x%x\n", linear_addr.addr); \
  printf("cpu.cr3.page_directory_base = 0x%x\n", cpu.cr3.page_directory_base << 12); \
  printf("dir = 0x%x\n", linear_addr.dir); \
  printf("page = 0x%x\n", linear_addr.page); \
  printf("offset = 0x%x\n", linear_addr.offset); \
  printf("pde = 0x%x\n", pde.val); \

  //printf("pte = 0x%x\n", pte.val); 

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
  //printf("------------------\n");
  //printf("linear_addr = 0x%x\n", linear_addr.addr);
  //printf("dir = 0x%x\n", linear_addr.dir);
  //printf("page = 0x%x\n", linear_addr.page);
  //printf("offset = 0x%x\n", linear_addr.offset);
  //printf("cpu.cr3.page_directory_base = 0x%x\n", cpu.cr3.page_directory_base << 12);
  //printf("dir = 0x%x\n", (cpu.cr3.page_directory_base << 12) + linear_addr.dir * 4);

  pde.val = paddr_read((cpu.cr3.page_directory_base << 12) + linear_addr.dir * 4, 4);
  //printf("pde = 0x%x\n", pde.val);
  if (!pde.present) {
    print_debug_info
  }
  assert(pde.present);

  pte.val = paddr_read((pde.page_frame << 12) + linear_addr.page * 4, 4);
  if (!pte.present) {
    print_debug_info
    printf("pte = 0x%x\n", pte.val);
  }
  //printf("pte = 0x%x\n", pte.val);
  assert(pte.present);

  //panic("untested codes here!\n");
  //printf("paddr = 0x%x\n", (pte.page_frame << 12) + linear_addr.offset);
  //printf("mem = 0x%x\n", paddr_read((pte.page_frame << 12) + linear_addr.offset, 4));
  return (pte.page_frame << 12) + linear_addr.offset;
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  //printf("addr = 0x%x\n", addr);
  if (cpu.cr0.paging) {
    if ((addr & 0xfff) + len > PAGE_SIZE) {
      // cross pages
      //panic("cross pages!");
      Log("cross pages!");
      paddr_t paddr = page_translate(addr);
      return paddr_read(paddr, len);
    }
    else {
      // not cross pages
      paddr_t paddr = page_translate(addr);
      return paddr_read(paddr, len);
    }
  }
  else
    return paddr_read(addr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if (cpu.cr0.paging) {
    if ((addr & 0xfff) + len > PAGE_SIZE) {
      // cross pages
      panic("cross pages!");
      // Log("cross pages!");
    }
    else {
      // not cross pages
      paddr_t paddr = page_translate(addr);
      paddr_write(paddr, data, len);
    }
  }
  else
    paddr_write(addr, data, len);
}
