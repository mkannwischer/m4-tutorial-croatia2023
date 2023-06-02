#include <stdlib.h>
#include <stdio.h>
#include "hal.h"
#include "randombytes.h"
#include "ref.h"

#define Q 3329

int test_mulmod(void){
  int16_t a,b;
  int32_t bprime;
  int16_t cref, c;
  randombytes(&a, sizeof(int16_t));

  // twiddle factor for the first NTT layer
  // 1729 ^ 4 = 1 mod q
  b = 1729;

  // Pre-computed for Plantard multiplication
  // Mod((-b*2**32 % 3329)* 3329^-1, 2**32)
  bprime = 2230699446;

  c = mulmod(a, bprime);
  cref = mulmod_ref(a, b);


  unsigned char str[100];
  sprintf(str, "%d, %d", c, cref);
  hal_send_str(str);

  if((c-cref) % Q == 0){
    return 0;
  } else {
    return -1;
  }
}

int test_butterfly(void){
  int16_t a[2];
  int16_t aref[2];
  int16_t b;
  int32_t bprime;
  randombytes(a, sizeof(a));
  memcpy(aref, a, sizeof(a));

  // twiddle factor for the first NTT layer
  // 1729 ^ 4 = 1 mod q
  b = 1729;

  // Pre-computed for Plantard multiplication
  // Mod((-b*2**32 % 3329)* 3329^-1, 2**32)
  bprime = 2230699446;
  butterfly(a, bprime);
  butterfly_ref(aref, b);


  for(int i=0;i<2;i++){
    if((a[i]-aref[i])%Q != 0){
      return -1;
    }
  }
  return 0;
}

int test_nttlayer1(void){
  int16_t a[256];
  int16_t aref[256];
  int err = 0;
  randombytes(a, sizeof(a));

  // need sure inputs are small enough
  for(int i=0;i<256;i++){
    a[i] %= Q;
  }

  memcpy(aref, a, sizeof(a));

  nttlayer1(a);
  nttlayer1_ref(aref);

  for(int i=0;i<256;i++){
    if((a[i]-aref[i])%Q != 0){
      // unsigned char str[100];
      // sprintf(str, "%d %d <-> %d", i, a[i], aref[i]);
      // hal_send_str(str);
      err = -1;
    }
  }
  return err;
}

static const int32_t twiddles_asm[] = {2230699446,3328631909,4243360600,3408622288,812805467,2447447570,1094061961,1370157786,2475831253,249002310,1028263423,3594406395,4205945745,734105255,2252632292,381889553,3157039644,1727534158,1904287092,3929849920,72249375,2889974991,1719793153,1839778722,2701610550,690239563,3718262466,3087370604,3714391964,2546790461,1059227441,372858381,427045412,4196914574,2265533966,1544330386,2972545705,2937711185,2651294021,838608815,2550660963,3242190693,815385801,3696329620,42575525,1703020977,2470670584,2991898216,1851390229,1041165097,583155668,1855260731,3700200122,1979116802,3098982111,3415073125,3376368103,1910737929,836028480,3191874164,4012420634,1583035408,1174052340,21932846,3562152210,752167598,3417653460,2112004045,932791035,2951903026,1419184148,1817845876,3434425636,4233039261,300609006,975366560,2781600929,3889854731,3935010590,2197155094,2130066389,3598276897,2308109491,2382939200,1228239371,1884934581,3466679822,1211467195,2977706375,3144137970,3080919767,945692709,3015121229,345764865,826997308,2043625172,2964804700,2628071007,4154339049,483812778,3288636719,2696449880,2122325384,1371447954,411563403,3577634219,976656727,2708061387,723783916,3181552825,3346694253,3617629408,1408862808,519937465,1323711759,1474661346,2773859924,3580214553,1143088323,2221668274,1563682897,2417773720,1327582262,2722253228,3786641338,1141798155,2779020594};

int test_ntt(void){
  int16_t a[256];
  int16_t aref[256];
  int err = 0;
  randombytes(a, sizeof(a));

  // need sure inputs are small enough
  for(int i=0;i<256;i++){
    a[i] %= Q;
  }
  memcpy(aref, a, sizeof(a));

  ntt(a, twiddles_asm);
  ntt_ref(aref);


  for(int i=0;i<256;i++){
    if((a[i]-aref[i])%Q != 0){
      err = -1;
    }
  }
  return err;
}


int main(void){
  hal_setup(CLOCK_BENCHMARK);
  hal_send_str("====== START ======");

  if(test_mulmod()){
    hal_send_str("ERROR MULMOD");
  } else {
    hal_send_str("OK MULMOD");
  }

  hal_send_str("#");

  if(test_butterfly()){
    hal_send_str("ERROR BUTTERFLY");
  } else {
    hal_send_str("OK BUTTERFLY");
  }

  hal_send_str("#");

  if(test_nttlayer1()){
    hal_send_str("ERROR NTTLAYER1");
  } else {
    hal_send_str("OK NTTLAYER1");
  }

  hal_send_str("#");

  if(test_ntt()){
    hal_send_str("ERROR NTT");
  } else {
    hal_send_str("OK NTT");
  }

  hal_send_str("====== END ======");

  return 0;
}
