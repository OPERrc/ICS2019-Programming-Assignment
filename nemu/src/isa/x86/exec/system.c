#include "cpu/exec.h"
void raise_intr(uint32_t, vaddr_t);

make_EHelper(lidt) {
  // Log("0x%x\n", id_dest->addr);
  rtl_lm(&s0, &id_dest->addr, 2);
  // Log("0x%x\n", s0);
  rtl_mv(&cpu.idtr.limit, &s0);

  rtl_addi(&id_dest->addr, &id_dest->addr, 2);
  rtl_lm(&s0, &id_dest->addr, 4);
  // Log("0x%x\n", s0);
  rtl_mv(&cpu.idtr.base, &s0);
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  raise_intr(id_dest->val, cpu.pc);

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {
  switch (id_src->width) {
    case 4: s0 = pio_read_l(id_src->val); break;
    case 2: s0 = pio_read_w(id_src->val); break;
    case 1: s0 = pio_read_b(id_src->val); break;
    default: assert(0);
  }
  operand_write(id_dest, &s0);

  print_asm_template2(in);
}

make_EHelper(out) {
  switch (id_src->width) {
    case 4: pio_write_l(id_dest->val, id_src->val); break;
    case 2: pio_write_w(id_dest->val, id_src->val); break;
    case 1: pio_write_b(id_dest->val, id_src->val); break;
    default: assert(0);
  }

  print_asm_template2(out);
}
