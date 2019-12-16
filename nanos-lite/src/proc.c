#include "proc.h"

#define MAX_NR_PROC 4
#define TIME_CHANGE 10

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
void naive_uload(PCB *, const char *);
void context_kload(PCB *pcb, void *entry);
void context_uload(PCB *pcb, const char *filename);
int proc_time;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
  proc_time = 0;
  context_uload(&pcb[0], "/bin/pal");
  context_uload(&pcb[1], "/bin/hello");
  //context_kload(&pcb[1], (void *)hello_fun);
  //switch_boot_pcb();
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here

  //naive_uload(NULL, "/bin/init");
}

_Context* schedule(_Context *prev) {
  current->cp = prev;
  //current = &pcb[0];
  proc_time = (proc_time + 1) % TIME_CHANGE;
  current = (proc_time == 0 ? &pcb[1] : &pcb[0]);
  //printf("current->cp = 0x%x\n", current->cp);
  //assert(0);
  return current->cp;
}

void kill(PCB *pcb) {
  for (int i = 0; i < STACK_SIZE; i++)
    pcb->stack[i] = 0;
}
