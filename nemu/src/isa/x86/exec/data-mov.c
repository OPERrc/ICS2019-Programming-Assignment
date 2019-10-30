#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  if (id_dest->width == 4)
	  rtl_push(&id_dest->val);
  else if (id_dest->width == 1) {
    rtl_sext(&s0, &id_dest->val, 4);
    rtl_push(&s0);
  }
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  // rtl_lr(&id_dest->val, R_ESP, 4);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  // Log("ebp = 0x%x, esp = 0x%x\n", cpu.ebp, cpu.esp);
  rtl_lr(&cpu.esp, R_EBP, 4);
  // Log("ebp = 0x%x, esp = 0x%x\n", cpu.ebp, cpu.esp);
  rtl_pop(&cpu.ebp);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    if (cpu.gpr[R_AX]._16 >> 15 == 1) {
      rtl_li(&s0, 0xffff);
      rtl_sr(R_DX, &s0, 2);
    }
    else {
      rtl_li(&s0, 0);
      rtl_sr(R_DX, &s0, 2);
    }
  }
  else {
    if (cpu.eax >> 31 == 1) {
      rtl_li(&s0, 0xffffffff);
      rtl_sr(R_EDX, &s0, 4);
    }
    else {
      rtl_li(&s0, 0);
      rtl_sr(R_EDX, &s0, 4);
    }
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  // Log("id_dest = 0x%x, id_src = 0x%x, id_src->width = %d\n", s0, id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
