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
  void isa_reg_display();
make_EHelper(mov_r2cr) {
  //printf("gpr[%d]:0x%x to cr%d:0x%x\n", id_src->reg, id_src->val, id_dest->reg, cpu.cr[id_dest->reg]);
  isa_reg_display();
  //assert(0);
  cpu.cr[id_dest->reg] = id_src->val;
  isa_reg_display();
  //printf("cr3->val = 0x%x\n", cpu.cr3.val);

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  //printf("cr%d:0x%x to gpr[%d]:0x%x\n", id_src->reg, cpu.cr[id_src->reg], id_dest->reg, reg_l(id_dest->reg));
  //assert(0);
  isa_reg_display();
  reg_l(id_dest->reg) = cpu.cr[id_src->reg];
  isa_reg_display();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  raise_intr(id_dest->val, decinfo.seq_pc);

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  rtl_pop(&decinfo.jmp_pc);
  rtl_jr(&decinfo.jmp_pc);
  rtl_pop(&cpu.cs);
  rtl_pop(&cpu.eflags);

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
