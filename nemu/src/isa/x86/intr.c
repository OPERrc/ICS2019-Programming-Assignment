#include "rtl/rtl.h"

#define IRQ_TIMER 32

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags);
  cpu.IF = 0;
  cpu.TF = 0;
  rtl_push(&cpu.cs);
  rtl_push(&ret_addr);
  // Log("0x%x\n", ret_addr);
  s0 = vaddr_read(cpu.idtr.base + NO * 8, 2);
  s1 = vaddr_read(cpu.idtr.base + NO * 8 + 6, 2);
  s1 = s1 << 16 | s0;
  rtl_jr(&s1);
  // Log("0x%x\n", cpu.pc);
}

bool isa_query_intr(void) {
  if (cpu.INTR == true) {
    cpu.INTR = false;
    raise_intr(IRQ_TIMER, cpu.pc);
    return true;
  }
  return false;
}
