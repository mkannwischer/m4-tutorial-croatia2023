#include "ref.h"

#define Q 3329

int16_t mulmod_ref(int16_t a, int16_t b){
    int32_t t = (int32_t)a*b;
    return t % Q;
}


void butterfly_ref(int16_t a[2], int16_t twiddle){
    int16_t t = mulmod_ref(a[1], twiddle);
    a[1] = a[0] - t;
    a[0] = a[0] + t;
}

void nttlayer1_ref(int16_t a[256]){
    int16_t twiddle = 1729;
    for(int i=0; i<128;i++){
        int16_t t = mulmod_ref(a[128+i], twiddle);
        a[128+i] = a[i] - t;
        a[i] = a[i] + t;
    }
}

// be aware that these twiddles are not in Montgomery domain!
static const int16_t zetas[] = {1729,2580,3289,2642,630,1897,848,1062,1919,193,797,2786,3260,569,1746,296,2447,1339,1476,3046,56,2240,1333,1426,2094,535,2882,2393,2879,1974,821,289,331,3253,1756,1197,2304,2277,2055,650,1977,2513,632,2865,33,1320,1915,2319,1435,807,452,1438,2868,1534,2402,2647,2617,1481,648,2474,3110,1227,910,17,2761,583,2649,1637,723,2288,1100,1409,2662,3281,233,756,2156,3015,3050,1703,1651,2789,1789,1847,952,1461,2687,939,2308,2437,2388,733,2337,268,641,1584,2298,2037,3220,375,2549,2090,1645,1063,319,2773,757,2099,561,2466,2594,2804,1092,403,1026,1143,2150,2775,886,1722,1212,1874,1029,2110,2935,885,2154};
void ntt_ref(int16_t a[256]){
  unsigned int len, start, j, k;
  int16_t zeta, t;

  k = 0;
  for(len = 128; len > 1; len >>= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k++];
      for(j = start; j < start + len; ++j) {
        t = mulmod_ref(a[j + len], zeta);
        a[j + len] = a[j] - t;
        a[j] = a[j] + t;
      }
    }
  }
}
