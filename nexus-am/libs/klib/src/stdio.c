#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char *itoa(int value, char *str, int radix) {
  assert(radix > 16);
  char index[] = "0123456789abcdef";
  int len = 0;
  while (value > 0) {
    str[len] = index[value % radix];
    value /= radix;
    len++;
  }
  str[len] = '\0';
  for (int i = 0; i <= len / 2; i++){
    char tmp = str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = tmp;
  }
  return str;
}

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  char a[1000];
  char *str = a;
  char *tmp = out;

  va_start(args, fmt);
  int flag = 0;
  while (*fmt) {
    if (*fmt != '%' && !flag) {
      *tmp++ = *fmt;
      fmt++;
      continue;
    }

    flag = 1;
    switch (*fmt) {
      case 's':
        str = va_arg(args, char *);
        while (*str) {
          *tmp++ = *str;
          str++;
        }
        flag = 0;
        break;
      
      case 'd':
        str = itoa(va_arg(args, int), str, 10);
        while (*str) {
          *tmp++ = *str;
          str++;
        }
        flag = 0;
        break;

      default: break;
    }
    fmt++;
  }
  //*tmp = '\0';
  va_end(args);

  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
