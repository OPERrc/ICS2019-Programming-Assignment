#include <ndl.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

int NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h);
void draw_sync();
static int screen_w, screen_h;

static void get_display_info() {
  FILE *dispinfo = fopen("/proc/dispinfo", "r");
  assert(dispinfo);
  screen_w = screen_h = 0;
  char buf[128], key[128], value[128], *delim;
  while (fgets(buf, 128, dispinfo)) {
    *(delim = strchr(buf, ':')) = '\0';
    sscanf(buf, "%s", key);
    sscanf(delim + 1, "%s", value);
    if (strcmp(key, "WIDTH") == 0) sscanf(value, "%d", &screen_w);
    if (strcmp(key, "HEIGHT") == 0) sscanf(value, "%d", &screen_h);
  }
  fclose(dispinfo);
  assert(screen_w > 0 && screen_h > 0);
}

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      info->width = screen_w;
      info->height = screen_h;
      // info->width = 400;
      // info->height = 300;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
      uint32_t *pixels = ctl->pixels;
      NDL_DrawRect(pixels, x, y, w, h);
      if (ctl->sync) {
        NDL_Render();
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  get_display_info();
}
