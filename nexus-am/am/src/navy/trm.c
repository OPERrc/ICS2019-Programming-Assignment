#include <am.h>
#include <stdlib.h>
#include <stdio.h>

_Area _heap = {};

int main(const char *args);

void _trm_init() {
  int ret = main(NULL);
  _halt(ret);
}

void _putc(char ch) {
  putchar(ch);
}

void _halt(int code) {
  printf("Exit(%d)\n", code);
  exit(code);
  while (1);
}
