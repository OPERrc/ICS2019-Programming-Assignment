#include "cpu/exec.h"

// arith.c
make_EHelper(sub); // eflags TODO()
make_EHelper(add); // eflags TODO()
make_EHelper(cmp); // eflags TODO()
make_EHelper(inc); // eflags TODO()
make_EHelper(adc); // eflags TODO()
make_EHelper(dec); // eflags TODO()
make_EHelper(imul2);

// control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);

// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(leave);
make_EHelper(movzx);
make_EHelper(pop);
make_EHelper(cltd);

// logic.c
make_EHelper(xor); // eflags TODO()
make_EHelper(and); // eflags TODO()
make_EHelper(setcc); // eflags TODO()
make_EHelper(test); // eflags TODO()
make_EHelper(or); // eflags TODO()
make_EHelper(sar); // eflags TODO()
make_EHelper(shl); // eflags TODO()
make_EHelper(not);

make_EHelper(operand_size);

// special.c
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);
