#include "common.h"
#include "syscall.h"

void sys_yield(_Context *c) {
  _yield();
  c->GPRx = 0;
}

void sys_exit(_Context *c) {
  _halt(c->GPR2);
}

void sys_write(_Context *c) {
  if (c->GPR2 != 1 && c->GPR2 != 2)
    c->GPRx = -1;
  else {
    char *buf = (char *)c->GPR3;
    uintptr_t len = 0;
    while(len < c->GPR4 && buf) {
      _putc(*buf++);
      len++;
    }
    c->GPRx = len;
  }
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_yield:
      sys_yield(c);
      break;
    case SYS_write:
      sys_write(c);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
