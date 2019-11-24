#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  int num = 0;
  //Log("%s\n", buf);
  while (buf && num < len) {
    _putc(*(char *)buf++);
    num++;
  }
  return num;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  int down = 0;
  int num = 0;
  if (key & 0x8000) {
    key ^= 0x8000;
    down = 1;
  }
  if (key != _KEY_NONE) {
    char name[128];
    switch (down) {
      case 0:
        sprintf(name, "ku %s\n", keyname[key]);
        num = 0;
        while (name[num] && num < len) {
          *(char *)buf++ = name[num];
          num++;
        }
        return num;
        break;
      case 1:
        sprintf(name, "kd %s\n", keyname[key]);
        num = 0;
        while (name[num] && num < len) {
          *(char *)buf++ = name[num];
          num++;
        }
        return num;
        break;
      default: assert(0);
    }
  }

  char time[128];
  num = 0;
  sprintf(time, "t\n", uptime());
  while (time[num] && num < len) {
    *(char *)buf++ = time[num];
    num++;
  }
  return num;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //printf("in dispinfo: dispinfo = %s\n", &dispinfo[offset]);
  size_t num = 0;
  while (dispinfo[offset + num] && num < len) {
    *(char *)buf++ = dispinfo[offset + num];
    num++;
  }
  // size_t num = len;
  // memcpy(buf, &dispinfo[offset], num);
  // printf("in dispinfo: num = %d\n", num);
  return num;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int width = screen_width();
  int height = screen_height();
  // printf("%d, %d\n", screen_width(), screen_height());
  int x = (int)offset / width;
  int y = offset - x * width;
  //while (len > 0) {
    draw_rect((uint32_t *)buf, x, y, width, height);
    //len -= 8;
  //}
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
  // printf("%s\n", dispinfo);
}
