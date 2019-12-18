#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "assert.h"
#include "stdio.h"

typedef int FLOAT;

static inline int F2int(FLOAT a) {
  FLOAT result = a >> 16;
  return result;
}

static inline FLOAT int2F(int a) {
  FLOAT result = a << 16;
  //printf("a = 0x%x\n", a);
  //printf("result = 0x%x\n", result);
  //assert(0);
  return result;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
  FLOAT result = a * b;
  return result;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
  //assert(0);
  FLOAT result = a / b;
  return result;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT Fsqrt(FLOAT);
FLOAT Fpow(FLOAT, FLOAT);

#endif
