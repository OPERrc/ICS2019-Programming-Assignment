#include "cpu/exec.h"

// arith.c
make_EHelper(sub); // eflags TODO()
make_EHelper(add); // eflags TODO()
make_EHelper(cmp); // eflags relate to sub

// control.c
make_EHelper(call);
make_EHelper(ret);

// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);

// logic.c
make_EHelper(xor); // eflags TODO()
make_EHelper(and); // eflags TODO()
make_EHelper(setcc);
make_EHelper(test);

make_EHelper(operand_size);

// special.c
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);
