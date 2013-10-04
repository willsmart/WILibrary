//>Maker.cpp

#ifndef CONSTANTSH
#define CONSTANTSH


#define PI (3.1415926535897932384626433832795)
#define floatPI (3.1415926535897932384626433832795f)

float RandNormal(float mean,float sd);
double normalDensity(double x,double mean,double sd);
float Rand0to1();
long RandInt(long maxPlusOne);
#ifdef WIN32
#define Rand() (((rand()&0x7fff)<<15)|(rand()&0x7fff))
#define RandDWRD() ((((DWRD)(rand()))<<28)|(Rand()&0xfffffff))
#define RandULL() ((((ULL)RandDWRD())<<32)|RandDWRD())
#define SRand(rn) srand(rn)
#define Rand_MAX 0x3fffffff
#else
long Rand();
#define SRand(rn) srandom(rn)
#define RandDWRD() ((((DWRD)(rand()))<<28)|(Rand()&0xfffffff))
#define RandULL() ((((ULL)RandDWRD())<<32)|RandDWRD())
#define Rand_MAX RAND_MAX
#endif
extern long primes[];
extern DWRD shiftOne[32];// binary one shifted [index] places
extern ULL shiftOneULL[64];// binary one shifted [index] places
extern ULL leftOnes[65];
extern ULL rightOnes[65];
extern BYT numOnes[256];// number of ones in binary representation of numbers 0-255
extern BYT numOnes32(DWRD d);
extern BYT numOnes64(ULL ull);
extern long asciiToHex[256];
#define MAXHASHSTRLEN 1024
extern ULL charHashCodes[256][MAXHASHSTRLEN];
extern ULL hashCodeJustForBeingAString;
ULL makeStringHashCode(const char *str,char op='+');
ULL makeArrayHashCode(const void *str,long len,char op='+');
ULL makeArrayHashCode(const void *str,long len,char op,long sti,ULL ret);
ULL obfuscateHash(ULL hash);


#ifdef MAINCPPFILE
BYT numOnes32(DWRD d) {
  return(numOnes[d&0xff]+numOnes[(d>>8)&0xff]+numOnes[(d>>16)&0xff]+numOnes[(d>>24)&0xff]);
}
BYT numOnes64(ULL ull) {return(numOnes32((DWRD)(ull>>32))+numOnes32((DWRD)(ull&0xffffffff)));}
ULL hashCodeJustForBeingAString=0x512e9f2a6d18bc4eLL;
ULL hashCodeJustForBeingAnArray=0x5768920198390193LL;

ULL makeStringHashCode(const char *str,char op) {
//  prnt("%p:%c ",str,op);
  ULL ret=hashCodeJustForBeingAString;
  if (strlen(str)>MAXHASHSTRLEN) {
    long mulpi=0;ULL mul=primes[mulpi];
    switch (op) {
      case '+':default:for (int i=0;str[i];) {ret+=mul*charHashCodes[(BYT)str[i]][i];if (!((++i)&(MAXHASHSTRLEN-1))) mul=primes[++mulpi];}break;
      case '^':for (int i=0;str[i];) {ret^=mul*charHashCodes[(BYT)str[i]][i];if (!((++i)&(MAXHASHSTRLEN-1))) mul=primes[++mulpi];}break;
      case '*':ERROR("Can't have a long string with this op");
    }
  }
  else {
    switch (op) {
      case '+':default:for (int i=0;str[i];i++) ret+=charHashCodes[(BYT)str[i]][i];break;
      case '^':for (int i=0;str[i];i++) ret^=charHashCodes[(BYT)str[i]][i];break;
      case '*':for (int i=0;str[i];i++) ret*=charHashCodes[(BYT)str[i]][i];break;
    }
  }
  return(ret);
}
ULL makeArrayHashCode(const void *str,long len,char op) {
  BYT *b=(BYT*)str;
  ULL ret=hashCodeJustForBeingAnArray;
  if (len>MAXHASHSTRLEN) {
    long mulpi=0;ULL mul=primes[mulpi];
    switch (op) {
      case '+':default:for (int i=0;i<len;) {ret+=mul*charHashCodes[b[i]][i&(MAXHASHSTRLEN-1)];if (!((++i)&(MAXHASHSTRLEN-1))) mul=primes[++mulpi];}break;
      case '^':for (int i=0;i<len;) {ret^=mul*charHashCodes[b[i]][i&(MAXHASHSTRLEN-1)];if (!((++i)&(MAXHASHSTRLEN-1))) mul=primes[++mulpi];}break;
      case '*':ERROR("Can't have a long string with this op");
    }
  }
  else {
    switch (op) {
      case '+':default:for (int i=0;i<len;i++) ret+=charHashCodes[b[i]][i];break;
      case '^':for (int i=0;i<len;i++) ret^=charHashCodes[b[i]][i];break;
      case '*':for (int i=0;i<len;i++) ret*=charHashCodes[b[i]][i];break;
    }
  }
  return(ret);
}
ULL makeArrayHashCode(const void *str,long len,char op,long sti,ULL ret) {
  BYT *b=(BYT*)str;
  if (len+sti>MAXHASHSTRLEN) {
    long mulpi=sti/MAXHASHSTRLEN;ULL mul=primes[mulpi];
    switch (op) {
      case '+':default:for (int i=0;i<len;) {ret+=mul*charHashCodes[b[i]][i+sti];if (!(((++i)+sti)&(MAXHASHSTRLEN-1))) mul=primes[++mulpi];}break;
      case '^':for (int i=0;i<len;) {ret^=mul*charHashCodes[b[i]][i+sti];if (!(((++i)+sti)&(MAXHASHSTRLEN-1))) mul=primes[++mulpi];}break;
      case '*':ERROR("Can't have a long string with this op");
    }
  }
  else {
    switch (op) {
      case '+':default:for (int i=0;i<len;i++) ret+=charHashCodes[b[i]][i+sti];break;
      case '^':for (int i=0;i<len;i++) ret^=charHashCodes[b[i]][i+sti];break;
      case '*':for (int i=0;i<len;i++) ret*=charHashCodes[b[i]][i+sti];break;
    }
  }
  return(ret);
}


ULL obfuscateHash(ULL hash) {
  BYT *bp=(BYT*)&hash;
  return(charHashCodes[bp[0]][bp[1]]^
    (charHashCodes[bp[2]][bp[3]]*charHashCodes[0][0x100])^
    (charHashCodes[bp[4]][bp[5]]*charHashCodes[0][0x101])^
    (charHashCodes[bp[6]][bp[7]]*charHashCodes[0][0x102]));
}

#ifdef MTCHC
ULL charHashCodes[256][1024];
void makeCHC() {
  for (long i=0;i<256;i++) for (long j=0;j<1024;j++) charHashCodes[i][j]=RandULL();
}
#else
#include "../../BigData/charHashCodes.h"
#endif

double g_sqrtTwoPi=sqrt(2.0*PI);

float RandNormal(float mean,float sd) {
  float u1,u2,r,theta,v,pi=3.141592654f;

  u1 = float(Rand())/Rand_MAX;
  u2 = float(Rand())/Rand_MAX;

  r = float(sqrt(-2.0f*log(u1)));
  theta =2.0f*pi*u2;
  v = float(r*cos(theta));
// v2= r*sin(theta);
  return(sd*v+mean);
}

double normalDensity(double x,double mean,double sd) {return(exp(-(x-mean)*(x-mean)/(2.0*sd*sd))/(sd*g_sqrtTwoPi));}

float Rand0to1() {
  double d=Rand();
  return(float(d/long(Rand_MAX)));
}

long RandInt(long maxPlusOne) {
  return(Rand()%maxPlusOne);
}

#ifndef WIN32
long Rand() {return(random());}
#endif

long primes[]={
  2,      3,      5,      7,      11,     13,     17,     19,     23,     29, 
  31,     37,     41,     43,     47,     53,     59,     61,     67,     71, 
  73,     79,     83,     89,     97,     101,    103,    107,    109,    113, 
  127,    131,    137,    139,    149,    151,    157,    163,    167,    173, 
  179,    181,    191,    193,    197,    199,    211,    223,    227,    229, 
  233,    239,    241,    251,    257,    263,    269,    271,    277,    281, 
  283,    293,    307,    311,    313,    317,    331,    337,    347,    349, 
  353,    359,    367,    373,    379,    383,    389,    397,    401,    409, 
  419,    421,    431,    433,    439,    443,    449,    457,    461,    463, 
  467,    479,    487,    491,    499,    503,    509,    521,    523,    541, 
  547,    557,    563,    569,    571,    577,    587,    593,    599,    601, 
  607,    613,    617,    619,    631,    641,    643,    647,    653,    659, 
  661,    673,    677,    683,    691,    701,    709,    719,    727,    733, 
  739,    743,    751,    757,    761,    769,    773,    787,    797,    809, 
  811,    821,    823,    827,    829,    839,    853,    857,    859,    863, 
  877,    881,    883,    887,    907,    911,    919,    929,    937,    941, 
  947,    953,    967,    971,    977,    983,    991,    997,   1009,   1013, 
  1019,   1021,   1031,   1033,   1039,   1049,   1051,   1061,   1063,   1069, 
  1087,   1091,   1093,   1097,   1103,   1109,   1117,   1123,   1129,   1151, 
  1153,   1163,   1171,   1181,   1187,   1193,   1201,   1213,   1217,   1223, 
  1229,   1231,   1237,   1249,   1259,   1277,   1279,   1283,   1289,   1291, 
  1297,   1301,   1303,   1307,   1319,   1321,   1327,   1361,   1367,   1373, 
  1381,   1399,   1409,   1423,   1427,   1429,   1433,   1439,   1447,   1451, 
  1453,   1459,   1471,   1481,   1483,   1487,   1489,   1493,   1499,   1511, 
  1523,   1531,   1543,   1549,   1553,   1559,   1567,   1571,   1579,   1583, 
  1597,   1601,   1607,   1609,   1613,   1619,   1621,   1627,   1637,   1657, 
  1663,   1667,   1669,   1693,   1697,   1699,   1709,   1721,   1723,   1733, 
  1741,   1747,   1753,   1759,   1777,   1783,   1787,   1789,   1801,   1811, 
  1823,   1831,   1847,   1861,   1867,   1871,   1873,   1877,   1879,   1889, 
  1901,   1907,   1913,   1931,   1933,   1949,   1951,   1973,   1979,   1987, 
  1993,   1997,   1999,   2003,   2011,   2017,   2027,   2029,   2039,   2053, 
  2063,   2069,   2081,   2083,   2087,   2089,   2099,   2111,   2113,   2129, 
  2131,   2137,   2141,   2143,   2153,   2161,   2179,   2203,   2207,   2213, 
  2221,   2237,   2239,   2243,   2251,   2267,   2269,   2273,   2281,   2287, 
  2293,   2297,   2309,   2311,   2333,   2339,   2341,   2347,   2351,   2357, 
  2371,   2377,   2381,   2383,   2389,   2393,   2399,   2411,   2417,   2423, 
  2437,   2441,   2447,   2459,   2467,   2473,   2477,   2503,   2521,   2531, 
  2539,   2543,   2549,   2551,   2557,   2579,   2591,   2593,   2609,   2617, 
  2621,   2633,   2647,   2657,   2659,   2663,   2671,   2677,   2683,   2687, 
  2689,   2693,   2699,   2707,   2711,   2713,   2719,   2729,   2731,   2741, 
  2749,   2753,   2767,   2777,   2789,   2791,   2797,   2801,   2803,   2819, 
  2833,   2837,   2843,   2851,   2857,   2861,   2879,   2887,   2897,   2903, 
  2909,   2917,   2927,   2939,   2953,   2957,   2963,   2969,   2971,   2999, 
  3001,   3011,   3019,   3023,   3037,   3041,   3049,   3061,   3067,   3079, 
  3083,   3089,   3109,   3119,   3121,   3137,   3163,   3167,   3169,   3181, 
  3187,   3191,   3203,   3209,   3217,   3221,   3229,   3251,   3253,   3257, 
  3259,   3271,   3299,   3301,   3307,   3313,   3319,   3323,   3329,   3331, 
  3343,   3347,   3359,   3361,   3371,   3373,   3389,   3391,   3407,   3413, 
  3433,   3449,   3457,   3461,   3463,   3467,   3469,   3491,   3499,   3511, 
  3517,   3527,   3529,   3533,   3539,   3541,   3547,   3557,   3559,   3571, 
  3581,   3583,   3593,   3607,   3613,   3617,   3623,   3631,   3637,   3643, 
  3659,   3671,   3673,   3677,   3691,   3697,   3701,   3709,   3719,   3727, 
  3733,   3739,   3761,   3767,   3769,   3779,   3793,   3797,   3803,   3821, 
  3823,   3833,   3847,   3851,   3853,   3863,   3877,   3881,   3889,   3907, 
  3911,   3917,   3919,   3923,   3929,   3931,   3943,   3947,   3967,   3989, 
  4001,   4003,   4007,   4013,   4019,   4021,   4027,   4049,   4051,   4057, 
  4073,   4079,   4091,   4093,   4099,   4111,   4127,   4129,   4133,   4139, 
  4153,   4157,   4159,   4177,   4201,   4211,   4217,   4219,   4229,   4231, 
  4241,   4243,   4253,   4259,   4261,   4271,   4273,   4283,   4289,   4297, 
  4327,   4337,   4339,   4349,   4357,   4363,   4373,   4391,   4397,   4409, 
  4421,   4423,   4441,   4447,   4451,   4457,   4463,   4481,   4483,   4493, 
  4507,   4513,   4517,   4519,   4523,   4547,   4549,   4561,   4567,   4583, 
  4591,   4597,   4603,   4621,   4637,   4639,   4643,   4649,   4651,   4657, 
  4663,   4673,   4679,   4691,   4703,   4721,   4723,   4729,   4733,   4751, 
  4759,   4783,   4787,   4789,   4793,   4799,   4801,   4813,   4817,   4831, 
  4861,   4871,   4877,   4889,   4903,   4909,   4919,   4931,   4933,   4937, 
  4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003, 
  5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087, 
  5099,   5101,   5107,   5113,   5119,   5147,   5153,   5167,   5171,   5179, 
  5189,   5197,   5209,   5227,   5231,   5233,   5237,   5261,   5273,   5279, 
  5281,   5297,   5303,   5309,   5323,   5333,   5347,   5351,   5381,   5387, 
  5393,   5399,   5407,   5413,   5417,   5419,   5431,   5437,   5441,   5443, 
  5449,   5471,   5477,   5479,   5483,   5501,   5503,   5507,   5519,   5521, 
  5527,   5531,   5557,   5563,   5569,   5573,   5581,   5591,   5623,   5639, 
  5641,   5647,   5651,   5653,   5657,   5659,   5669,   5683,   5689,   5693, 
  5701,   5711,   5717,   5737,   5741,   5743,   5749,   5779,   5783,   5791, 
  5801,   5807,   5813,   5821,   5827,   5839,   5843,   5849,   5851,   5857, 
  5861,   5867,   5869,   5879,   5881,   5897,   5903,   5923,   5927,   5939, 
  5953,   5981,   5987,   6007,   6011,   6029,   6037,   6043,   6047,   6053, 
  6067,   6073,   6079,   6089,   6091,   6101,   6113,   6121,   6131,   6133, 
  6143,   6151,   6163,   6173,   6197,   6199,   6203,   6211,   6217,   6221, 
  6229,   6247,   6257,   6263,   6269,   6271,   6277,   6287,   6299,   6301, 
  6311,   6317,   6323,   6329,   6337,   6343,   6353,   6359,   6361,   6367, 
  6373,   6379,   6389,   6397,   6421,   6427,   6449,   6451,   6469,   6473, 
  6481,   6491,   6521,   6529,   6547,   6551,   6553,   6563,   6569,   6571, 
  6577,   6581,   6599,   6607,   6619,   6637,   6653,   6659,   6661,   6673, 
  6679,   6689,   6691,   6701,   6703,   6709,   6719,   6733,   6737,   6761, 
  6763,   6779,   6781,   6791,   6793,   6803,   6823,   6827,   6829,   6833, 
  6841,   6857,   6863,   6869,   6871,   6883,   6899,   6907,   6911,   6917, 
  6947,   6949,   6959,   6961,   6967,   6971,   6977,   6983,   6991,   6997, 
  7001,   7013,   7019,   7027,   7039,   7043,   7057,   7069,   7079,   7103, 
  7109,   7121,   7127,   7129,   7151,   7159,   7177,   7187,   7193,   7207,
  7211,   7213,   7219,   7229,   7237,   7243,   7247,   7253,   7283,   7297, 
  7307,   7309,   7321,   7331,   7333,   7349,   7351,   7369,   7393,   7411, 
  7417,   7433,   7451,   7457,   7459,   7477,   7481,   7487,   7489,   7499, 
  7507,   7517,   7523,   7529,   7537,   7541,   7547,   7549,   7559,   7561, 
  7573,   7577,   7583,   7589,   7591,   7603,   7607,   7621,   7639,   7643, 
  7649,   7669,   7673,   7681,   7687,   7691,   7699,   7703,   7717,   7723, 
  7727,   7741,   7753,   7757,   7759,   7789,   7793,   7817,   7823,   7829, 
  7841,   7853,   7867,   7873,   7877,   7879,   7883,   7901,   7907,   7919}; 

DWRD shiftOne[32] = {// binary one shifted [index] places
  0x1,0x2,0x4,0x8,                          0x10,0x20,0x40,0x80,
  0x100,0x200,0x400,0x800,                  0x1000,0x2000,0x4000,0x8000,
  0x10000,0x20000,0x40000,0x80000,          0x100000,0x200000,0x400000,0x800000,
  0x1000000,0x2000000,0x4000000,0x8000000,  0x10000000,0x20000000,0x40000000,0x80000000};
ULL shiftOneULL[64] = {// binary one shifted [index] places
  0x1,0x2,0x4,0x8,                          0x10,0x20,0x40,0x80,
  0x100,0x200,0x400,0x800,                  0x1000,0x2000,0x4000,0x8000,
  0x10000,0x20000,0x40000,0x80000,          0x100000,0x200000,0x400000,0x800000,
  0x1000000,0x2000000,0x4000000,0x8000000,  
  0x10000000  ,0x20000000  ,0x40000000  ,0x80000000,
  0x100000000LL,0x200000000LL,0x400000000LL,0x800000000LL,                          
  0x1000000000LL,0x2000000000LL,0x4000000000LL,0x8000000000LL,
  0x10000000000LL,0x20000000000LL,0x40000000000LL,0x80000000000LL,                  
  0x100000000000LL,0x200000000000LL,0x400000000000LL,0x800000000000LL,
  0x1000000000000LL,0x2000000000000LL,0x4000000000000LL,0x8000000000000LL,          
  0x10000000000000LL,0x20000000000000LL,0x40000000000000LL,0x80000000000000LL,
  0x100000000000000LL,0x200000000000000LL,0x400000000000000LL,0x800000000000000LL,  
  0x1000000000000000LL,0x2000000000000000LL,0x4000000000000000LL,0x8000000000000000LL
};
ULL leftOnes[65]={0,
    0x8000000000000000LL,0xc000000000000000LL,0xe000000000000000LL,0xf000000000000000LL,
    0xf800000000000000LL,0xfc00000000000000LL,0xfe00000000000000LL,0xff00000000000000LL,
    0xff80000000000000LL,0xffc0000000000000LL,0xffe0000000000000LL,0xfff0000000000000LL,
    0xfff8000000000000LL,0xfffc000000000000LL,0xfffe000000000000LL,0xffff000000000000LL,

    0xffff800000000000LL,0xffffc00000000000LL,0xffffe00000000000LL,0xfffff00000000000LL,
    0xfffff80000000000LL,0xfffffc0000000000LL,0xfffffe0000000000LL,0xffffff0000000000LL,
    0xffffff8000000000LL,0xffffffc000000000LL,0xffffffe000000000LL,0xfffffff000000000LL,
    0xfffffff800000000LL,0xfffffffc00000000LL,0xfffffffe00000000LL,0xffffffff00000000LL,

    0xffffffff80000000LL,0xffffffffc0000000LL,0xffffffffe0000000LL,0xfffffffff0000000LL,
    0xfffffffff8000000LL,0xfffffffffc000000LL,0xfffffffffe000000LL,0xffffffffff000000LL,
    0xffffffffff800000LL,0xffffffffffc00000LL,0xffffffffffe00000LL,0xfffffffffff00000LL,
    0xfffffffffff80000LL,0xfffffffffffc0000LL,0xfffffffffffe0000LL,0xffffffffffff0000LL,

    0xffffffffffff8000LL,0xffffffffffffc000LL,0xffffffffffffe000LL,0xfffffffffffff000LL,
    0xfffffffffffff800LL,0xfffffffffffffc00LL,0xfffffffffffffe00LL,0xffffffffffffff00LL,
    0xffffffffffffff80LL,0xffffffffffffffc0LL,0xffffffffffffffe0LL,0xfffffffffffffff0LL,
    0xfffffffffffffff8LL,0xfffffffffffffffcLL,0xfffffffffffffffeLL,0xffffffffffffffffLL
};
ULL rightOnes[65]={0,
    0x0000000000000001LL,0x0000000000000003LL,0x0000000000000007LL,0x000000000000000fLL,
    0x000000000000001fLL,0x000000000000003fLL,0x000000000000007fLL,0x00000000000000ffLL,
    0x00000000000001ffLL,0x00000000000003ffLL,0x00000000000007ffLL,0x0000000000000fffLL,
    0x0000000000001fffLL,0x0000000000003fffLL,0x0000000000007fffLL,0x000000000000ffffLL,

    0x000000000001ffffLL,0x000000000003ffffLL,0x000000000007ffffLL,0x00000000000fffffLL,
    0x00000000001fffffLL,0x00000000003fffffLL,0x00000000007fffffLL,0x0000000000ffffffLL,
    0x0000000001ffffffLL,0x0000000003ffffffLL,0x0000000007ffffffLL,0x000000000fffffffLL,
    0x000000001fffffffLL,0x000000003fffffffLL,0x000000007fffffffLL,0x00000000ffffffffLL,

    0x00000001ffffffffLL,0x00000003ffffffffLL,0x00000007ffffffffLL,0x0000000fffffffffLL,
    0x0000001fffffffffLL,0x0000003fffffffffLL,0x0000007fffffffffLL,0x000000ffffffffffLL,
    0x000001ffffffffffLL,0x000003ffffffffffLL,0x000007ffffffffffLL,0x00000fffffffffffLL,
    0x00001fffffffffffLL,0x00003fffffffffffLL,0x00007fffffffffffLL,0x0000ffffffffffffLL,

    0x0001ffffffffffffLL,0x0003ffffffffffffLL,0x0007ffffffffffffLL,0x000fffffffffffffLL,
    0x001fffffffffffffLL,0x003fffffffffffffLL,0x007fffffffffffffLL,0x00ffffffffffffffLL,
    0x01ffffffffffffffLL,0x03ffffffffffffffLL,0x07ffffffffffffffLL,0x0fffffffffffffffLL,
    0x1fffffffffffffffLL,0x3fffffffffffffffLL,0x7fffffffffffffffLL,0xffffffffffffffffLL
};
 
BYT numOnes[256] = {// number of ones in binary representation of numbers 0-255
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

long asciiToHex[256] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, 
  -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

ULL factorial[20]={
    1,2,6,24,120,720,5040,40320,362880,3628800,39916800,479001600,6227020800,87178291200,
    1307674368000,20922789888000,355687428096000,6402373705728000,121645100408832000,2432902008176640000
};
//    51090942171709440000,1124000727777607680000,25852016738884976640000,620448401733239439360000,
//    15511210043330985984000000,403291461126605635584000000,10888869450418352160768000000,
//    304888344611713860501504000000,8841761993739701954543616000000,265252859812191058636308480000000,
//    8222838654177922817725562880000000,263130836933693530167218012160000000
//};

#endif

#endif


