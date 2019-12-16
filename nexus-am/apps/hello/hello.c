#include <am.h>

static inline void putstr(const char *s) {
  for (; *s; s ++) _putc(*s);
}

int main() {
  for (int i = 0; i < 10; i ++) {
    putstr("Hello, AM World @ " __ISA__ "\n");
  }
  return 0;
}
