#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //excluding '\0'
  size_t i;
  for (i = 0; s[i] != '\0'; i++);
  //printf("it's strlen\n");
  return i;
}

char *strcpy(char* dst,const char* src) {
  // include '\0'
  size_t i;
  for (i = 0; src[i] != '\0'; i++) 
    dst[i] = src[i];
  dst[i] = '\0';
  //printf("it's strcpt\n");
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[i] = src[i];
  for ( ; i < n; i++)
    dst[i] = '\0';
  //printf("it's strcpy\n");
  return dst;
}

char* strcat(char* dst, const char* src) {
  size_t dest_len = strlen(dst);
  size_t i;
  for ( i = 0; src[i] != '\0'; i++)
    dst[dest_len + i] = src[i];
  //printf("it's strcat\n");
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  size_t i;
  for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
  {
    if (s1[i] < s2[i]) return -1;
    if (s1[i] > s2[i]) return 1;
  }
  if (s1[i] != '\0') return 1;
  if (s2[i] != '\0') return -1;
  //printf("it's strcmp\n");
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  size_t i;
  for (i = 0; i < n && s1[i] != '\0' && s2[i] != '\0'; i++)
  {
    if (s1[i] < s2[i]) return -1;
    if (s1[i] > s2[i]) return 1;
  }
  if (i == n) return 0;
  if (s1[i] != '\0') return 1;
  if (s2[i] != '\0') return -1;
  //printf("it's strncmp\n");
  return 0;
}

void* memset(void* v,int c,size_t n) {
  size_t i;
  for (i = 0; i < n; i++)
    *((char *)(v + i)) = c;
  //printf("it's memset\n");
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  size_t i;
  char *res = (char *)out;
  char *src = (char *)in;
  for (i = 0; i < n; i++)
    res[i] = src[i];
  //printf("it's memcpy\n");
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  size_t i;
  for (i = 0; i < n; i++)
  {
    if (*((char *)s1 + i) > *((char *)s2 + i)) return 1;
    if (*((char *)s1 + i) < *((char *)s2 + i)) return -1;
  }
  //printf("it's memcmp\n");
  return 0;
}

#endif
