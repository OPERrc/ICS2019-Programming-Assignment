#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags);
  cpu.IF = 0;
  cpu.TF = 0;
  rtl_push(&cpu.cs);
  rtl_push(&cpu.pc);
  // Log("0x%x\n", NO);
  // TODO();
  cpu.pc = vaddr_read(cpu.idtr.base + (NO - 1) * 8, 4);
  Log("0x%x\n", cpu.pc);
  TODO();
}

bool isa_query_intr(void) {
  return false;
}
