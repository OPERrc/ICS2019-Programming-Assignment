#include <am.h>
#include <stdlib.h>
#include <stdio.h>

_Area _heap = {};

void _start(int argc, char *argv[], char *envp[]);

void _trm_init() {
  _start(0, NULL, NULL);
}

void _putc(char ch) {
  putchar(ch);
}

void _halt(int code) {
  printf("Exit(%d)\n", code);
  exit(code);
  while (1);
}
