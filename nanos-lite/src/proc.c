#include "proc.h"

#define MAX_NR_PROC 4
#define TIME_CHANGE 50

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
void naive_uload(PCB *, const char *);
void context_kload(PCB *pcb, void *entry);
void context_uload(PCB *pcb, const char *filename);
int proc_time;
int fg_pcb;

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
  /*proc_time = 0;
  fg_pcb = 3;
  context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[1], "/bin/litenes-am");
  context_uload(&pcb[2], "/bin/typing-am");
  context_uload(&pcb[3], "/bin/pal");
  //context_kload(&pcb[1], (void *)hello_fun);
  //switch_boot_pcb();
  switch_boot_pcb();*/

  Log("Initializing processes...");

  // load program here

  naive_uload(NULL, "/bin/pal");
}

_Context* schedule(_Context *prev) {
  current->cp = prev;
  //current = &pcb[1];
  proc_time = (proc_time + 1) % TIME_CHANGE;
  current = (proc_time == 0 ? &pcb[0] : &pcb[fg_pcb]);
  //printf("current->cp = 0x%x\n", current->cp);
  //assert(0);
  return current->cp;
}

