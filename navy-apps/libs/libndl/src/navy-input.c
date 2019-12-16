#include <ndl.h>
#include <stdio.h>
//#include <stddef.h>

int NDL_WaitEvent(NDL_Event *event);

size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      NDL_Event e;
      NDL_WaitEvent(&e);
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      //assert(0);
      if (e.type == NDL_EVENT_KEYDOWN || e.type == NDL_EVENT_KEYUP) {
        kbd->keydown = 1 - e.type;
        kbd->keycode = e.data;
      }
      else {
        kbd->keydown = 0;
        kbd->keycode = NDL_SCANCODE_NONE;
      }
      //printf("e.type = %d, key = %d, keydown = %d\n", e.type, kbd->keycode, kbd->keydown);
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
