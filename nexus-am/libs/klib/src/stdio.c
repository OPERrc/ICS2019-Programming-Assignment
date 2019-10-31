#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

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
  int d;

  va_start(args, fmt);
  int flag = 0;
  while (*fmt) {
    // _putc(*fmt);
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
          // _putc(*str);
          *tmp++ = *str;
          str++;
        }
        flag = 0;
        break;
      
      case 'd':
        // _putc('x');
        d = va_arg(args, int);
        str = itoa(d, str, 10);
        _putc('x');
        while (*str) {
          // _putc(*str);
          *tmp++ = *str;
          str++;
        }
        flag = 0;
        break;

      default: break;
    }
    fmt++;
  }
  *tmp = '\0';
  va_end(args);

  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
