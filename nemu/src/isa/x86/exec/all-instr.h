#include "cpu/exec.h"

// arith.c
make_EHelper(sub);

// control.c
make_EHelper(call);
make_EHelper(ret);

// data-mov.c
make_EHelper(mov);
make_EHelper(push);
make_EHelper(lea);

// logic.c
make_EHelper(xor);

make_EHelper(operand_size);

// special.c
make_EHelper(nop);
make_EHelper(inv);
make_EHelper(nemu_trap);
