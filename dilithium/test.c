#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "hal.h"
#include "randombytes.h"
#include "ref.h"

#define Q 8380417

int test_mulmod(void){
  int32_t a,b,bmont;
  int32_t cref, c;
  randombytes(&a, sizeof(int32_t));

  // twiddle factor for the first NTT layer
  // 4808194 ^ 4 = 1 mod q
  b = 4808194;
  bmont = 25847;

  c = mulmod(a, bmont);
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
  int32_t a[2];
  int32_t aref[2];
  int32_t b, bmont;
  randombytes(a, sizeof(a));
  memcpy(aref, a, sizeof(a));

  // twiddle factor for the first NTT layer
  // 4808194 ^ 4 = 1 mod q
  b = 4808194;
  bmont = 25847;

  butterfly(a, bmont);
  butterfly_ref(aref, b);


  for(int i=0;i<2;i++){
    if((a[i]-aref[i])%Q != 0){
      return -1;
    }
  }
  return 0;
}

int test_nttlayer1(void){
  int32_t a[256];
  int32_t aref[256];
  int err = 0;
  randombytes(a, sizeof(a));
  memcpy(aref, a, sizeof(a));

  nttlayer1(a);
  nttlayer1_ref(aref);

  for(int i=0;i<256;i++){
    if((a[i]-aref[i])%Q != 0){
      err = -1;
    }
  }
  return err;
}


const int32_t twiddles_asm[255] = {
     25847, -2608894,  -518909,   237124,  -777960,  -876248,   466468,
   1826347,  2353451,  -359251, -2091905,  3119733, -2884855,  3111497,  2680103,
   2725464,  1024112, -1079900,  3585928,  -549488, -1119584,  2619752, -2108549,
  -2118186, -3859737, -1399561, -3277672,  1757237,   -19422,  4010497,   280005,
   2706023,    95776,  3077325,  3530437, -1661693, -3592148, -2537516,  3915439,
  -3861115, -3043716,  3574422, -2867647,  3539968,  -300467,  2348700,  -539299,
  -1699267, -1643818,  3505694, -3821735,  3507263, -2140649, -1600420,  3699596,
    811944,   531354,   954230,  3881043,  3900724, -2556880,  2071892, -2797779,
  -3930395, -1528703, -3677745, -3041255, -1452451,  3475950,  2176455, -1585221,
  -1257611,  1939314, -4083598, -1000202, -3190144, -3157330, -3632928,   126922,
   3412210,  -983419,  2147896,  2715295, -2967645, -3693493,  -411027, -2477047,
   -671102, -1228525,   -22981, -1308169,  -381987,  1349076,  1852771, -1430430,
  -3343383,   264944,   508951,  3097992,    44288, -1100098,   904516,  3958618,
  -3724342,    -8578,  1653064, -3249728,  2389356,  -210977,   759969, -1316856,
    189548, -3553272,  3159746, -1851402, -2409325,  -177440,  1315589,  1341330,
   1285669, -1584928,  -812732, -1439742, -3019102, -3881060, -3628969,  3839961,
   2091667,  3407706,  2316500,  3817976, -3342478,  2244091, -2446433, -3562462,
    266997,  2434439, -1235728,  3513181, -3520352, -3759364, -1197226, -3193378,
    900702,  1859098,   909542,   819034,   495491, -1613174,   -43260,  -522500,
   -655327, -3122442,  2031748,  3207046, -3556995,  -525098,  -768622, -3595838,
    342297,   286988, -2437823,  4108315,  3437287, -3342277,  1735879,   203044,
   2842341,  2691481, -2590150,  1265009,  4055324,  1247620,  2486353,  1595974,
  -3767016,  1250494,  2635921, -3548272, -2994039,  1869119,  1903435, -1050970,
  -1333058,  1237275, -3318210, -1430225,  -451100,  1312455,  3306115, -1962642,
  -1279661,  1917081, -2546312, -1374803,  1500165,   777191,  2235880,  3406031,
   -542412, -2831860, -1671176, -1846953, -2584293, -3724270,   594136, -3776993,
  -2013608,  2432395,  2454455,  -164721,  1957272,  3369112,   185531, -1207385,
  -3183426,   162844,  1616392,  3014001,   810149,  1652634, -3694233, -1799107,
  -3038916,  3523897,  3866901,   269760,  2213111,  -975884,  1717735,   472078,
   -426683,  1723600, -1803090,  1910376, -1667432, -1104333,  -260646, -3833893,
  -2939036, -2235985,  -420899, -2286327,   183443,  -976891,  1612842, -3545687,
   -554416,  3919660,   -48306, -1362209,  3937738,  1400424,  -846154,  1976782
};


int test_ntt(void){
  uint64_t t0, t1;
  unsigned char str[100];
  int32_t a[256];
  int32_t aref[256];
  int err = 0;
  randombytes(a, sizeof(a));
  memcpy(aref, a, sizeof(a));

  t0 = hal_get_time();
  ntt(a, twiddles_asm);
  t1 = hal_get_time();
  sprintf(str,"Dilithium ntt: %llu cycles (note that these are meaningless on qemu)", t1-t0);
  hal_send_str(str);


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
