#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// void my_itoa(char *str, int digit);

int printf(const char *fmt, ...) {
  char buf[200];
  va_list(ap);
  va_start(ap, fmt);
  int res = vsprintf(buf, fmt, ap);
  for (int i = 0; i < res; i++) _putc(buf[i]);
  return res;
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
  *out = '\0';
	int index = 0;  // position in *fmt
  int count = 0;  // position in *out
  char *str; //the string to be added
  char val_str[50]; //string of val
  int val = 0; // the digit of %d
  char val_char;

  for (index = 0; fmt[index] != '\0'; index++)
  {
    if (fmt[index] != '%')
    {
  
      out[count] = fmt[index];
      count++;
      continue;
    }
  
    index++;
  
    switch(fmt[index])
    {
      case 's':
        // _putc('s');
        for (str = va_arg(ap, char*); *str != '\0'; str++)
        {
          // _putc(*str);
          out[count] = *str;
          count++;
        }
        break;

      case 'c':
        val_char = va_arg(ap, int);
        out[count] = (char)val_char;
        count++;
        break;
      
      case 'd':
      // _putc('d');
        val = va_arg(ap, int);
        my_itoa(val_str, val, 10);
        for (int i = 0; val_str[i] != '\0'; i++)
        {
          out[count] = val_str[i];
          count++;
        }
        break;
      
      case 'p':
        val = va_arg(ap, uint32_t);
        out[count] = '0'; count++;
        out[count] = 'x'; count++;
        my_itoa(val_str, val, 16);
        for (int i = 0; val_str[i] != '\0'; i++)
        {
          out[count] = val_str[i];
          count++;
        }
        break;
      
      case 'x':
        val = va_arg(ap, uint32_t);
        out[count] = '0'; count++;
        out[count] = 'x'; count++;
        my_itoa(val_str, val, 16);
        for (int i = 0; val_str[i] != '\0'; i++)
        {
          out[count] = val_str[i];
          count++;
        }
        break;

      default: break;
    }
    // _putc('\n');
  }

  out[count] = '\0';
  //count++;
  return count;
}

int sprintf(char *out, const char *fmt, ...) {
  // _putc('a');
  // _putc('\n');
  va_list ap;
  va_start(ap, fmt);
  int res = vsprintf(out, fmt, ap);
  va_end(ap);
  return res;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

void my_itoa(char *str, unsigned digit, int base){
  int count = 0;
  char buffer[30];
  char table[] = "0123456789abcde";
	
  if (digit == 0)
  {
    str[0] = '0';
    str[1] = '\0';
    return;
  }
  while (digit != 0){
	  buffer[count] = table[digit % base];
    digit /= base;
    count++;
  }
	
	for (int i = 0; i < count; i++) 
		str[i] = buffer[count - i - 1];
  str[count] = '\0';
  return;
}
#endif

