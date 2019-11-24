#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  // rtl_sext(&s0, &id_dest->val, id_dest->width);
  // rtl_push(&s0);
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  // rtl_lr(&id_dest->val, R_ESP, 4);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  rtl_mv(&s0, &cpu.esp);
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&s0);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);

  print_asm("pusha");
}

make_EHelper(popa) {
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&s0);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);

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
    if (((cpu.gpr[R_AX]._16 >> 15) & 1) == 1) {
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
    rtl_sext(&s0, &cpu.eax, 1);
    rtl_sr(R_AX, &s0, 2);
  }
  else {
    rtl_sext(&s0, &cpu.eax, 2);
    rtl_sr(R_EAX, &s0, 4);
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

make_EHelper(movs) {
  // id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  // rtl_sext(&s0, &id_src->val, id_src->width);
  // Log("width = 0x%x, id_src = 0x%x, id_src->width = %d\n", decinfo.src.width, id_src->val, id_src->width);
  // operand_write(id_dest, &s0);
  // void isa_reg_display();
  // isa_reg_display();
  vaddr_write(cpu.edi, vaddr_read(cpu.esi, id_dest->width), id_dest->width);
  cpu.edi += id_dest->width;
  cpu.esi += id_dest->width;
  // Log("DIFF_TEST passed!\n");
  // assert(0);
  print_asm_template2(movs);
}
