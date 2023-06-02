#ifndef _REF_H_
#define _REF_H_

#include <stdint.h>

int32_t mulmod_ref(int32_t a, int32_t b);
void butterfly_ref(int32_t a[2], int32_t twiddle);
void nttlayer1_ref(int32_t a[256]);
void ntt_ref(int32_t a[256]);


#endif