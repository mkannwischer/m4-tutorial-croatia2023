#ifndef _REF_H_
#define _REF_H_

#include <stdint.h>

int16_t mulmod_ref(int16_t a, int16_t b);
void butterfly_ref(int16_t a[2], int16_t twiddle);
void nttlayer1_ref(int16_t a[256]);
void ntt_ref(int16_t a[256]);


#endif