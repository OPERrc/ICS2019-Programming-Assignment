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
  char name[128];
  char time[128];
  int down = 0;
  int num = 0;
  if (key & 0x8000) {
    key ^= 0x8000;
    down = 1;
  }
  if (key != _KEY_NONE) {
    //printf("name before = %s\n", name);
    if (down) 
      sprintf(name, "kd %s\n", keyname[key]);
    else
      sprintf(name, "ku %s\n", keyname[key]);
    num = 0;
    //printf("name after = %s\n", name);
    while (name[num] && num < len) {
      *(char *)buf++ = name[num];
      num++;
    }
    return num;
  } else {
    num = 0;
    sprintf(time, "t %d\n", uptime());
    while (time[num] && num < len) {
      *(char *)buf++ = time[num];
      num++;
    }
    return num;
  }
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //printf("in dispinfo: dispinfo = %s\n", &dispinfo[offset]);
  // printf("in disp %d\n", offset);
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
  uint32_t *pixels = (uint32_t *)buf;
  offset /= 4;
  int width = screen_width();
  int height = screen_height();
  // printf("offset = %d, len = %d\n", offset, len);
  int y = offset / width;
  int x = offset % width;
  // printf("before: x = %d, y = %d, len = %d\n", x, y, len);
  //while (len > 0) {
  if (offset + len > width * height)
    len = width * height - offset;
  len /= 4;
  for (int i = 0; i < len; i++) {
    draw_rect(&pixels[i], x, y, 1, 1);
    x = (x + 1) % width;
    if (x == 0)
      y++;
  }
  // printf("after: x = %d, y = %d\n", x, y);
    //len -= 8;
  //}
  return len * 4;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
  // printf("%s\n", dispinfo);
}
