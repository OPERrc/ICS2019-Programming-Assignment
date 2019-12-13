#include "common.h"

_Context* do_syscall(_Context*);
_Context* schedule(_Context *prev);

static _Context* do_event(_Event e, _Context* c) {
  _Context *next;
  switch (e.event) {
    case _EVENT_YIELD: 
      printf("Event \"YIELD\" catched!\n"); 
      next =  schedule(c);
      break;
    case _EVENT_SYSCALL: 
      // printf("Event \"SYSCALL\" catched!\n"); 
      next = do_syscall(c);
      break; 
    default: panic("Unhandled event ID = %d", e.event);
  }

  return next;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
