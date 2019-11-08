#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  // uintptr_t eax, ecx, edx, ebx, esp, ebp, esi, edi, eflags, cs, eip;
  int irq;
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax, eflags, cs, eip;
  // uintptr_t esi, ebx, eax, eip, edx, eflags, ecx, cs, esp, edi, ebp;
  struct _AddressSpace *as;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
