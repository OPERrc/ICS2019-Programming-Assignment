#include "common.h"

_Context* do_syscall(_Context*);
_Context* schedule(_Context *prev);

static _Context* do_event(_Event e, _Context* c) {
  //_Context *next;
  switch (e.event) {
    case _EVENT_YIELD: 
      printf("Event \"YIELD\" catched!\n"); 
      return schedule(c);
      break;
    case _EVENT_SYSCALL: 
      // printf("Event \"SYSCALL\" catched!\n"); 
      do_syscall(c);
      break; 
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
