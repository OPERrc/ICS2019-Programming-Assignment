#include "cpu/exec.h"

CPU_state cpu;

rtlreg_t s0, s1, t0, t1, ir;

/* shared by all helper functions */
DecodeInfo decinfo;

void decinfo_set_jmp(bool is_jmp) {
  decinfo.is_jmp = is_jmp;
}

void isa_exec(vaddr_t *pc);
bool isa_query_intr();

vaddr_t exec_once(void) {
  //printf("cpu.pc = 0x%x\n", cpu.pc);
  decinfo.seq_pc = cpu.pc;
  isa_exec(&decinfo.seq_pc);
  if (!isa_query_intr())
    update_pc();

  return decinfo.seq_pc;
}
