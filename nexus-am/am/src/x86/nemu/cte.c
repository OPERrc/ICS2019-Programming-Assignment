#include <am.h>
#include <x86.h>
#include <klib.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

void __am_irq0();
void __am_vecsys();
void __am_vectrap();
void __am_vecnull();

void Context_display(_Context *c) {
  printf("-------------------\n");
  printf("eax = 0x%x\n", c->eax);
  printf("ecx = 0x%x\n", c->ecx);
  printf("edx = 0x%x\n", c->edx);
  printf("ebx = 0x%x\n", c->ebx);
  printf("esp = 0x%x\n", c->esp);
  printf("ebp = 0x%x\n", c->ebp);
  printf("esi = 0x%x\n", c->esi);
  printf("edi = 0x%x\n", c->edi);
  printf("eflags = 0x%x\n", c->eflags);
  printf("cs = 0x%x\n", c->cs);
  printf("pc = 0x%x\n", c->eip);
  printf("irq = 0x%x\n", c->irq);
  printf("as = 0x%x\n", c->as);
  printf("-------------------\n");
}

void __am_get_cur_as(_Context *c);
void __am_switch(_Context *c);

_Context* __am_irq_handle(_Context *c) {
  //Context_display(c);
  __am_get_cur_as(c);
  //Context_display(c);
  _Context *next = c;
  
  //Context_display(c);

  if (user_handler) {
    _Event ev = {0};
    switch (c->irq) {
      case 32: my_TODO(); ev.event = _EVENT_IRQ_TIMER; break;
      case 0x80: ev.event = _EVENT_SYSCALL; break;
      case 0x81: ev.event = _EVENT_YIELD; break; 
      default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
    //printf("ev.event = %d, next_context = 0x%x\n", ev.event, next);
  }
  //assert(0);
  //Context_display(next);
  __am_switch(next);
  return next;
}

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  static GateDesc idt[NR_IRQ];

  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vecnull, DPL_KERN);
  }

  // ----------------------- interrupts ----------------------------
  idt[32]   = GATE(STS_IG32, KSEL(SEG_KCODE), __am_irq0,   DPL_KERN);
  // ---------------------- system call ----------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vecsys, DPL_USER);
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vectrap, DPL_KERN);

  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  /**(_Context *)stack.start = *(_Context *)entry;
  printf("entry = 0x%x\n", *(_Context *)entry);
  printf("*stack.start = 0x%x\n", *(_Context *)stack.start);
  printf("stack.start = 0x%x\n", stack.start);
  printf("in cte.c: _kcontext(): TODO\n");*/
  //assert(0);
  _Context *new = stack.end - sizeof(_Context);
  //printf("stack.end = 0x%x\n", stack.end);
  //printf("*new = 0x%x\n", new);
  //printf("context size = %d\n", sizeof(_Context));
  new->eip = (uintptr_t)entry;
  new->cs = 8;
  //printf("entry = 0x%x\n", new->eip);
  //printf("in cte.c: _kcontext(): TODO\n");
  //assert(0);
  return new;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
