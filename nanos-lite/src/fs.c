#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);


typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
  {"/dev/fb", 0, 0, invalid_read, fb_write},
#include "files.h"
  {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode) {
  int i;
  printf("pathname = %s\n", pathname);
  for (i = 0; i < NR_FILES; i++)
    if (strcmp(pathname, file_table[i].name) == 0)
      break;
  // printf("name = %s\n", file_table[i].name);
  if (i == NR_FILES)
    assert(0);
  return i;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  assert(offset <= file_table[fd].size);
  switch(whence) {
    case SEEK_SET: file_table[fd].open_offset = offset; break;
    case SEEK_CUR: file_table[fd].open_offset += offset; break;
    case SEEK_END: file_table[fd].open_offset = offset + file_table[fd].size; break;
    default: assert(0); return -1;
  }
  return file_table[fd].open_offset;
}

size_t fs_read(int fd, void *buf, size_t len) {
  assert(file_table[fd].open_offset <= file_table[fd].size);
  if (fd >= NR_FILES)
    return -1;
  printf("name = %s\n", file_table[fd].name);
  // printf("file_table[fd].read == NULL? %d\n", file_table[fd].read == NULL);
  if (file_table[fd].read == NULL) {
    size_t off = ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    // printf("open_offset_before = %d\n", file_table[fd].open_offset);
    fs_lseek(fd, off, SEEK_CUR);
    // printf("open_offset_after = %d\n", file_table[fd].open_offset);
    return len;
  }
  else
    return file_table[fd].read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  // printf("fd = %d, name = %s, size = %d\n", fd, file_table[fd].name, file_table[fd].size);
}

size_t fs_write(int fd, const void *buf, size_t len) {
  assert(file_table[fd].open_offset <= file_table[fd].size);

  if (file_table[fd].write == NULL) {
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    fs_lseek(fd, len, SEEK_CUR);
    return len;
  }
  else
    return file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  /*
  if (fd != 1 && fd != 2) {
    file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    fs_lseek(fd, len, SEEK_CUR);
    return len;
  }
  else {
    // Log("%s\n", buf);
    uintptr_t i = 0;
    while(i < len && buf) {
      _putc(*(char *)buf++);
      i++;
    }
    return i;
  }*/
}

int fs_close(int fd) {
  return 0;
}

void init_fs() {
  file_table[1].write = &serial_write;
  file_table[2].write = &serial_write;
  for (int i = 4; i < NR_FILES - 1; i++) {
    file_table[i].read = NULL;
    file_table[i].write = NULL;
    file_table[i].open_offset = 0;
  }
  
  // TODO: initialize the size of /dev/fb
  // file_table[NR_FILES-1].size = (screen_width() << 16) | (screen_height());
  file_table[FD_FB].size = screen_width() * screen_height();
}
