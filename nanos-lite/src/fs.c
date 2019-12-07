#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);


size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t fbsync_write(const void *buf, size_t offset, size_t len);


typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_FBSYNC, FD_TTY};

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
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, invalid_read, fb_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
  {"/proc/dispinfo", 128, 0, dispinfo_read, invalid_write},
  {"/dev/fbsync", 0, 0, invalid_read, fbsync_write},
  {"/dev/tty", 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

size_t fs_lseek(int fd, size_t offset, int whence);

int fs_open(const char *pathname, int flags, int mode) {
  int i;
  printf("pathname = %s\n", pathname);
  for (i = 0; i < NR_FILES; i++)
    if (strcmp(pathname, file_table[i].name) == 0)
      break;
  // printf("name = %s\n", file_table[i].name);
  if (i == NR_FILES)
    assert(0);
  fs_lseek(i, 0, SEEK_SET);
  return i;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  //printf("name = %s, offset = %d, whence = %d\n", file_table[fd].name, offset, whence);
  switch(whence) {
    case SEEK_SET: assert(offset <= file_table[fd].size); file_table[fd].open_offset = offset; break;
    case SEEK_CUR: assert(file_table[fd].open_offset + offset <= file_table[fd].size); file_table[fd].open_offset += offset; break;
    case SEEK_END: assert(offset == 0); file_table[fd].open_offset = offset + file_table[fd].size; break;
    default: assert(0); return -1;
  }
  return file_table[fd].open_offset;
}

size_t fs_read(int fd, void *buf, size_t len) {
  assert(file_table[fd].open_offset <= file_table[fd].size);
  if (fd >= NR_FILES) {
    assert(0);
    return -1;
  }
  // printf("name = %s\n", file_table[fd].name);
  if (file_table[fd].read == NULL) {
    size_t off = 0;
    if (file_table[fd].open_offset + len > file_table[fd].size)
      len = file_table[fd].size - file_table[fd].open_offset;
    off = ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    fs_lseek(fd, off, SEEK_CUR);
    return off;
  }
  else {
    size_t off = file_table[fd].read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    if (fd == FD_DISPINFO)
      fs_lseek(fd, off, SEEK_CUR);
    return off;
  }
  // printf("fd = %d, name = %s, size = %d\n", fd, file_table[fd].name, file_table[fd].size);
}

size_t fs_write(int fd, const void *buf, size_t len) {
  assert(file_table[fd].open_offset <= file_table[fd].size);
  if (file_table[fd].write == NULL) {
    if (len + file_table[fd].open_offset > file_table[fd].size)
      len = file_table[fd].size - file_table[fd].open_offset;
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    fs_lseek(fd, len, SEEK_CUR);
    return len;
  }
  else {
    size_t off = file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    if (fd == FD_FB)
      fs_lseek(fd, off, SEEK_CUR);
    return off;
  }
}

int fs_close(int fd) {
  fs_lseek(fd, 0, SEEK_SET);
  return 0;
}

void init_fs() {
  file_table[FD_FB].size = 4 * screen_width() * screen_height();
}
