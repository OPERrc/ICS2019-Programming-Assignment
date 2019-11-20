#include "common.h"
#include "syscall.h"

void sys_yield(_Context *c) {
  _yield();
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  // printf("%d\n", c->irq);

  switch (a[0]) {
    case 0x80:
    case 0x81:
      sys_yield(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
