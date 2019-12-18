#include "FLOAT.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

FLOAT F_mul_F(FLOAT a, FLOAT b) {
  //assert(0);
  //printf("------a * b-----\n");
  //printf("a = 0x%x\n", a);
  //printf("b = 0x%x\n", b);
  //printf("a*b = 0x%x\n", a*b);
  //printf("a*b/2^16 = 0x%x\n", a*b/(2^16));
  //printf("a/2^8 * b/2^8 = 0x%x\n", (a >> 8) * (b >> 8));
  FLOAT result = (a >> 8) * (b >> 8);
  //printf("a*b = 0x%x\n", result);
  //assert(0);
  return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
  //printf("a = 0x%x\n", a);
  //printf("b = 0x%x\n", b);
  FLOAT result = (a / b << 16);
  //printf("a/b = 0x%x\n", result);
  //assert(0);
  return result;
}

FLOAT f2F(float a) {
  /* You should figure out how to convert `a' into FLOAT without
   * introducing x87 floating point instructions. Else you can
   * not run this code in NEMU before implementing x87 floating
   * point instructions, which is contrary to our expectation.
   *
   * Hint: The bit representation of `a' is already on the
   * stack. How do you retrieve it to another variable without
   * performing arithmetic operations on it directly?
   */
  union {
    struct {
      uint32_t f :23;
      uint32_t e :8;
      uint32_t s :1;
    };
    float val;
    uint32_t i;
  } f;

  union {
    struct {
      uint32_t f :23;
      uint32_t e :9;
    };
    int32_t val;
  } i;

  union {
    struct {
      uint32_t f :16;
      uint32_t i :15;
      uint32_t s :1;
    };
    FLOAT val;
  } F;

  f.val = a;
  i.val = 0;
  F.val = 0;
  i.f = f.f;
  //printf("i.f = 0x%x\n", i.f);
  i.e = 1;
  int offset = f.e - 127;
  //printf("offset = %d\n", offset);
  if (offset > 0) {
    if (offset <= 9) {
      i.val = i.val << offset;
      F.i = i.e;
      F.f = i.f >> 7;
    }
    else {
      i.val = i.val << 9;
      F.i = i.e;
      F.f = i.f >> 7;
      F.val = F.val << (offset - 9);
    }
  }
  else {
    i.val = i.val >> (-offset);
    F.i = i.e;
    F.f = i.f >> 7;
  }
  F.s = f.s;
  /*
  printf("--------f to F--------\n");
  printf("float = 0x%x\n", f.i);
  printf("f.f = 0x%x\n", f.f);
  printf("f.e = 0x%x\n", f.e);
  printf("f.s = 0x%x\n", f.s);
  printf("FLOAT = 0x%x\n", F.val);
  */
  //assert(0);
  return F.val;
}

FLOAT Fabs(FLOAT a) {
  FLOAT result = 0 - a;
  //printf("a = 0x%x\n", a);
  //printf("abs(a) = 0x%x\n", result);
  return result;
}

/* Functions below are already implemented */

FLOAT Fsqrt(FLOAT x) {
  FLOAT dt, t = int2F(2);

  do {
    dt = F_div_int((F_div_F(x, t) - t), 2);
    t += dt;
  } while(Fabs(dt) > f2F(1e-4));

  return t;
}

FLOAT Fpow(FLOAT x, FLOAT y) {
  /* we only compute x^0.333 */
  FLOAT t2, dt, t = int2F(2);

  do {
    t2 = F_mul_F(t, t);
    dt = (F_div_F(x, t2) - t) / 3;
    t += dt;
  } while(Fabs(dt) > f2F(1e-4));

  return t;
}
