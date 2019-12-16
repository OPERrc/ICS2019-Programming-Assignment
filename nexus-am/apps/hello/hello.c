#include <am.h>

static inline void putstr(const char *s) {
  for (; *s; s ++) _putc(*s);
}

int main() {
  while(1) {
    putstr("Hello, AM World @ " __ISA__ "\n");
  }
  return 0;
}
