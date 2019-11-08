#include "cpu/exec.h"

// arith.c
make_EHelper(sub); // eflags TODO()
make_EHelper(add); // eflags TODO()
make_EHelper(cmp); // eflags TODO()
make_EHelper(inc); // eflags TODO()
make_EHelper(adc); // eflags TODO()
make_EHelper(dec); // eflags TODO()
make_EHelper(imul2);
make_EHelper(idiv);
make_EHelper(imul1);
make_EHelper(mul);
make_EHelper(sbb);
make_EHelper(div);
make_EHelper(neg);
make_EHelper(imul3);

// control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(call_rm);
make_EHelper(jmp_rm);

// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(leave);
make_EHelper(movzx);
make_EHelper(pop);
make_EHelper(cltd);
make_EHelper(movsx);
make_EHelper(cwtl);
make_EHelper(pusha);

// logic.c
make_EHelper(xor); // eflags TODO()
make_EHelper(and); // eflags TODO()
make_EHelper(setcc); // eflags TODO()
make_EHelper(test); // eflags TODO()
make_EHelper(or); // eflags TODO()
make_EHelper(sar); // eflags TODO()
make_EHelper(shl); // eflags TODO()
make_EHelper(not);
make_EHelper(shr);
make_EHelper(rol);

make_EHelper(operand_size);

// special.c
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);

// system.c
make_EHelper(out);
make_EHelper(in);
make_EHelper(lidt);
make_EHelper(int);
