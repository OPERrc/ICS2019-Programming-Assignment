#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (s[len])
    len++;
  return len;
}

char *strcpy(char* dst,const char* src) {
  size_t i = 0;
  while (src[i]) {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  my_TODO();
  return NULL;
}

char* strcat(char* dst, const char* src) {
  char *str = dst;
  while (*str)
    str++;
  size_t i = 0;
  while (src[i]) 
    *str++ = src[i++];

  *str = '\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  size_t i = 0;
  while (s1[i] && s2[i] && s1[i] == s2[i])
    i++;
  return s1[i] - s2[i];
}

int strncmp(const char* s1, const char* s2, size_t n) {
  my_TODO();
  return 0;
}

void* memset(void* v,int c,size_t n) {
  size_t *str = (size_t *)v;
  while (n--) 
    *str++ = c;
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  size_t *dest = (size_t *)out;
  size_t *src = (size_t *)in;
  while (n--) {
    *dest++ = *src;
    src++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  const size_t *str1 = (size_t *)s1;
  const size_t *str2 = (size_t *)s2;
  while (str1 && str2 && str1 == str2 && n--) {
    str1++;
    str2++;
  }
  return (int)(*str1 - *str2);
}

#endif
