#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  /*
  TEST computes the bit-wise logical AND of its two operands. 
  Each bit of the result is 1 if both of the corresponding bits of the operands are 1; 
  otherwise, each bit is 0. 
  The result of the operation is discarded and only the flags are modified. 
  */
 
  rtl_and(&s0, &id_dest->val, &id_src->val);
  // set ZF SF flags
  rtl_update_ZFSF(&s0, id_dest->width);
  // set CF OF flags
  rtl_li(&s1, 0);
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);

  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  // set ZF SF flags
  rtl_update_ZFSF(&s0, id_dest->width);
  // set CF OF flags
  rtl_li(&s1, 0);
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  print_asm_template2(and);
}

make_EHelper(xor) {
	rtl_xor(&s0, &id_dest->val, &id_src->val);
	operand_write(id_dest, &s0);
  // set ZF SF flags
  rtl_update_ZFSF(&s0, id_dest->width);
	// set CF OF flags
  rtl_li(&s1, 0);
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  // set ZF SF flags
  rtl_update_ZFSF(&s0, id_dest->width);
	// set CF OF flags
  rtl_li(&s1, 0);
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);

  print_asm_template2(or);
}

make_EHelper(sar) {
  rtl_sar(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  rtl_update_ZFSF(&s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  rtl_shl(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  rtl_update_ZFSF(&s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtl_shr(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  rtl_update_ZFSF(&s0, id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;
  rtl_setcc(&s0, cc);
  
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&s0, &id_dest->val);
  operand_write(id_dest, &s0);

  print_asm_template1(not);
}

make_EHelper(rol) {
  rtl_li(&s0, id_src->val);
  //printf("%d\n", id_src->val);
  //printf("%d\n", s0);
  while (s0--) {
    //printf("0x%x\n", id_dest->val);
    s1 = id_dest->val >> (id_dest->width * 8 - 1);
    id_dest->val = id_dest->val * 2 + s1;
    //operand_write(id_dest, &s1);
    cpu.CF = cpu.CF ^ s1;
  }
  operand_write(id_dest, &id_dest->val);
  if (id_src->val == 1) {
    if (id_dest->val >> (id_dest->width * 8 - 1) != cpu.CF)
      cpu.OF = 1;
    else
      cpu.OF = 0;
  }
}
