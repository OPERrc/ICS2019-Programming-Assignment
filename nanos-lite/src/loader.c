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

static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  Elf_Ehdr *ehdr = NULL;
  // Elf_Phdr *phdr = NULL;

  // read ehdr
  size_t point = 0;
  point += ramdisk_read(&ehdr->e_ident, point, sizeof(ehdr->e_ident));
  printf("%d\n", point);
  point += ramdisk_read(&ehdr->e_type, point, sizeof(ehdr->e_type));
  printf("%d\n", point);
  point += ramdisk_read(&ehdr->e_machine, point, sizeof(ehdr->e_machine));
  point += ramdisk_read(&ehdr->e_version, point, sizeof(ehdr->e_version));
  point += ramdisk_read(&ehdr->e_entry, point, sizeof(ehdr->e_entry));
  point += ramdisk_read(&ehdr->e_phoff, point, sizeof(ehdr->e_shoff));
  point += ramdisk_read(&ehdr->e_flags, point, sizeof(ehdr->e_flags));
  point += ramdisk_read(&ehdr->e_ehsize, point, sizeof(ehdr->e_ehsize));
  point += ramdisk_read(&ehdr->e_phentsize, point, sizeof(ehdr->e_phentsize));
  point += ramdisk_read(&ehdr->e_phnum, point, sizeof(ehdr->e_phnum));
  point += ramdisk_read(&ehdr->e_shentsize, point, sizeof(ehdr->e_shentsize));
  point += ramdisk_read(&ehdr->e_shnum, point, sizeof(ehdr->e_shnum));
  point += ramdisk_read(&ehdr->e_shstrndx, point, sizeof(ehdr->e_shstrndx));
  printf("%d\n", point);

  //read phdr
  return 0;
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
