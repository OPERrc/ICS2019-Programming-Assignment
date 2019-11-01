#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>

static inline int min(int x, int y) {
  return (x < y) ? x : y;
}

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      uint32_t wh = inl(SCREEN_ADDR);
      info->width = wh >> 16;
      info->height = wh & 0x0000ffff;
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
        uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
        int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
        uint32_t *pixels = ctl->pixels;
        uint32_t W = screen_width(), H = screen_height();
        // printf("W = %d, H = %d\n", W, H);
        // uint32_t W = 400, H = 300;
        int cp_bytes = sizeof(uint32_t) * min(w, W - x);
        for (int j = 0; j < h && y + j < H; j ++) {
          memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
          pixels += w;
        }
      if (ctl->sync) {
        //outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = i;
  draw_sync();
}
