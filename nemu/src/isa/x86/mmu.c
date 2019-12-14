#include "nemu.h"

paddr_t page_translate(vaddr_t addr) {
  union {
    struct{
      uint32_t OFFSET:12;
      uint32_t PAGE:10;
      uint32_t DIR:10;
    };
    vaddr_t addr;
  } linear_addr;

  union {
    struct{
      uint32_t P:1;
      uint32_t R_W:1;
      uint32_t U_S:1;
      uint32_t _1:2;
      uint32_t A:1;
      uint32_t D:1;
      uint32_t _2:2;
      uint32_t AVAIL:3;
      uint32_t PG_FRAME_ADDR:20;
    };
    vaddr_t addr;
  } PG_EBL_ENTRY;

  union {
    struct{
      uint32_t P:1;
      uint32_t R_W:1;
      uint32_t U_S:1;
      uint32_t _1:2;
      uint32_t A:1;
      uint32_t D:1;
      uint32_t _2:2;
      uint32_t AVAIL:3;
      uint32_t PG_TABLE_ADDR:20;
    };
    vaddr_t addr;
  } DIR_ENTRY;

  linear_addr.addr = addr;

  DIR_ENTRY.addr = paddr_read(cpu.cr3.page_directory_base + linear_addr.DIR * 4, 4);
  assert(DIR_ENTRY.P == 0);

  PG_EBL_ENTRY.addr = paddr_read(DIR_ENTRY.PG_TABLE_ADDR + linear_addr.PAGE * 4, 4);
  assert(PG_EBL_ENTRY.P == 0);

  panic("untested codes here!\n");
  return paddr_read(PG_EBL_ENTRY.PG_FRAME_ADDR + linear_addr.OFFSET * 4, 4);
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if (cpu.cr0.paging != 0) {
    printf("%d\n", cpu.cr0.paging);
    assert(0);
  }
  paddr_t paddr = (cpu.cr0.paging == 1 ? page_translate(addr) : addr);
  return paddr_read(paddr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_t paddr = (cpu.cr0.paging == 1 ? page_translate(addr) : addr);
  paddr_write(paddr, data, len);
}
