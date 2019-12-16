#include <ndl.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

extern FILE *evtdev;

#define keyname(k) #k,

static const char *keys[] = {
  "NONE",
  _KEYS(keyname)
};

#define numkeys ( sizeof(keys) / sizeof(keys[0]) )

int NDL_WaitKeyboard(NDL_Event *event) {
  char buf[256], *p = buf, ch;

  while (1) {
    while ((ch = getc(evtdev)) != -1) {
      *p ++ = ch;
      assert(p - buf < sizeof(buf));
      if (ch == '\n') break;
    }

    if (buf[0] == 'k') {
      char keyname[32];
      event->type = buf[1] == 'd' ? NDL_EVENT_KEYDOWN : NDL_EVENT_KEYUP;
      event->data = -1;
      sscanf(buf + 3, "%s", keyname);
      for (int i = 0; i < numkeys; i ++) {
        if (strcmp(keys[i], keyname) == 0) {
          event->data = i;
          break;
        }
      }
      assert(event->data >= 1 && event->data < numkeys);
      return 0;
    }
  }

  assert(0);
  return -1;
}

size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      NDL_Event e;
      NDL_WaitKeyboard(&e);
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
      printf("key = %d, keydown = %d\n", kbd->keycode, kbd->keydown);
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
