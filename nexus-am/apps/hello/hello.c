#include <am.h>
#include <stdio.h>

static inline void putstr(const char *s) {
  for (; *s; s ++) _putc(*s);
}

int main() {
  
  putstr("Hello, AM World @ " __ISA__ "\n");
  int i = 2;
  while(1) {
    putstr("Hello, AM World @ " __ISA__);
    printf(" for the %dth time!\n", i);
    i++;
  }
  return 0;
}
