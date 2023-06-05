#ifndef _M4_H_
#define _M4_H_

#include <stdint.h>

int32_t mulmod(int32_t a, int32_t b);
void butterfly(int32_t a[2], int32_t twiddle);
void nttlayer1(int32_t a[256]);
void ntt(int32_t a[256], const int32_t *twiddles_asm);


#endif