#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *, size_t, size_t);
size_t ramdisk_write(const void *, size_t, size_t);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;

  // read ehdr
  ramdisk_read(&ehdr, 0, sizeof(ehdr));

  size_t point = ehdr.e_phoff;
  printf("%d\n", ehdr.e_phnum);
  for (int i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
    point += ramdisk_read(&phdr, point, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      // size_t data[phdr.p_filesz];
      // ramdisk_read(&data, phdr.p_offset, phdr.p_filesz);
      uintptr_t *fb = (uintptr_t *)phdr.p_vaddr;
      ramdisk_read(fb, phdr.p_offset, phdr.p_filesz);
      memset(&fb[phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
      // ramdisk_write(&data, phdr.p_vaddr, phdr.p_memsz);
    }
  }
  
  return (uintptr_t)ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
