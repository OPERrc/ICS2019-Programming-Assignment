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

/*static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;

  // read ehdr
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &ehdr, sizeof(Elf_Ehdr));
  int point = ehdr.e_phoff;

  for (size_t i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
    point += fs_read(fd, &phdr, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      char *mem = (char *)phdr.p_vaddr;
      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      fs_read(fd, mem, phdr.p_filesz);
      fs_lseek(fd, point, SEEK_SET);
      memset(&mem[phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
    }
  }
  // size_t point = ehdr.e_phoff;
  //for (size_t i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
  //  point += ramdisk_read(&phdr, point, ehdr.e_phentsize);
  //  if (phdr.p_type == PT_LOAD) {
  //    uintptr_t *fb = (uintptr_t *)phdr.p_vaddr;
  //    ramdisk_read(fb, phdr.p_offset, phdr.p_filesz);
  //    memset(&fb[phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
  //  }
  //} 
  fs_close(fd);
  return (uintptr_t)ehdr.e_entry;
}*/
void* new_page(size_t nr_page);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;

  // read ehdr
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &ehdr, sizeof(Elf_Ehdr));
  int point = ehdr.e_phoff;

  for (size_t i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
    
    point += fs_read(fd, &phdr, ehdr.e_phentsize);

    if (phdr.p_type == PT_LOAD) {
      void *v_mem = (char *)phdr.p_vaddr;
      uintptr_t load_offset = 0;
      fs_lseek(fd, phdr.p_offset, SEEK_SET);

      // load pages
      while (load_offset <= phdr.p_memsz) {
        void *p_mem = new_page(1);
        _map(&(pcb->as), v_mem, p_mem, 0);

        if (load_offset + PGSIZE <= phdr.p_filesz)
          // in [0, filesz - PGSIZE]
          fs_read(fd, p_mem, PGSIZE);
        else if (load_offset <= phdr.p_filesz && phdr.p_filesz < load_offset + PGSIZE) {
          // cross filesz
          int len = phdr.p_filesz - load_offset;
          fs_read(fd, p_mem, len);
          memset(p_mem + len, 0, PGSIZE - len);
        }
        else if (load_offset + PGSIZE <= phdr.p_memsz)
          // in (filesz, memsz - PGSIZE]
          memset(p_mem, 0, PGSIZE);
        else
          // cross memsz
          memset(p_mem, 0, load_offset + PGSIZE - phdr.p_memsz);

        v_mem += PGSIZE;
        load_offset += PGSIZE;
      }

      fs_lseek(fd, point, SEEK_SET);
    }
  }
  // size_t point = ehdr.e_phoff;
  //for (size_t i = 0; i < ehdr.e_phnum; i++) {
    // read phdr
  //  point += ramdisk_read(&phdr, point, ehdr.e_phentsize);
  //  if (phdr.p_type == PT_LOAD) {
  //    uintptr_t *fb = (uintptr_t *)phdr.p_vaddr;
  //    ramdisk_read(fb, phdr.p_offset, phdr.p_filesz);
  //    memset(&fb[phdr.p_filesz], 0, phdr.p_memsz - phdr.p_filesz);
  //  }
  //} 
  fs_close(fd);
  //assert(0);
  return (uintptr_t)ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  //printf("here\n");
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  printf("pcb->stack = 0x%x\n", pcb->stack);
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
