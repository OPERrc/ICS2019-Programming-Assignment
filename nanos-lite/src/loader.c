#include "proc.h"
#include <elf.h>
#include "fs.h"

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *, size_t, size_t);
size_t ramdisk_write(const void *, size_t, size_t);
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;

  // read ehdr
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &ehdr, sizeof(ehdr));
  int point = ehdr.e_phoff;

  for (size_t i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
    point += fs_read(fd, &phdr, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      uintptr_t *mem = (uintptr_t *)phdr.p_vaddr;
      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      fs_read(fd, mem, phdr.p_filesz);
      fs_lseek(fd, point, SEEK_SET);
      memset(&mem[phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
    }
  }

  /* size_t point = ehdr.e_phoff;
  for (size_t i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
    point += ramdisk_read(&phdr, point, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      uintptr_t *fb = (uintptr_t *)phdr.p_vaddr;
      ramdisk_read(fb, phdr.p_offset, phdr.p_filesz);
      memset(&fb[phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
    }
  } */
  fs_close(fd);
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
