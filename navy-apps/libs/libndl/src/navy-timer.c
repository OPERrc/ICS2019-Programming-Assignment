#include <ndl.h>
//#include <stddef.h>

static uint32_t boot_time = 0;
int NDL_WaitEvent(NDL_Event *event);
uint32_t uptime();

size_t __am_timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      _DEV_TIMER_UPTIME_t *uptime = (_DEV_TIMER_UPTIME_t *)buf;
      NDL_Event e;
      while (e.data != NDL_EVENT_TIMER)
        NDL_WaitEvent(&e);
      uint32_t t = e.data;
      uptime->hi = 0;
      uptime->lo = t - boot_time;
      return sizeof(_DEV_TIMER_UPTIME_t);
    }
  }
  return 0;
}

void __am_timer_init() {
  boot_time = uptime();
}
