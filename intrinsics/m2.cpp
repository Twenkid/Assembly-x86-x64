#include <stdio.h>
//#include <emmintrin.h>
#include <new>
#include <immintrin.h>
#include <cstring>
#include <stdint.h>
#include <string.h>

#define SIZE 10000000
#define LEN 100

int len = LEN;

// Exercise with AVX intrinsics, gcc/g++ etc. on Windows WSL ... Recalling and learning
// 23-10-2022+
// Twenkid
// Inspired by a thread: https://stackoverflow.com/questions/72772188/memory-throughput-for-strided-memory-accesses
//Resources: see the end


void print128_num(__m128i var)
{
    uint16_t val[8];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %i %i %i %i %i %i %i %i \n", 
           val[0], val[1], val[2], val[3], val[4], val[5], 
           val[6], val[7]);
}

void p128d_num(__m128d in) {
    alignas(16) double val[2];  // uint64_t might give format-string warnings with %llx; it's just long in some ABIs
	memcpy(val, &in, sizeof(val));
    //_mm_store_sd128((__m128d*)v, in);
    printf("v2_u64: %f %f\n", val[0], val[1]); //reverse from set_pd(1.0, 2.0) ... ?
	
}


void print256_num(__m256i var)
{
    int32_t val[8];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %i %i %i %i %i %i %i %i \n", 
           val[0], val[1], val[2], val[3], val[4], val[5], 
           val[6], val[7]);
}

void print256(__m256 var)
{
    float val[8];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %f %f %f %f %f %f %f %f \n", 
           val[0], val[1], val[2], val[3], val[4], val[5], 
           val[6], val[7]);
}

void all(){ //#24-10-2022
 //The packed values arerepresented in right-to-left order, with thelowestvalue being used for scalar operations. Consider the following example operation:
  double a[2] = {1.0, 2.0};
 __m128d t = _mm_load_pd(a);
 __m128d t2 = _mm_set_pd(1.0, 2.0);
 __m128d t3 = _mm_set_pd(3.0, 10.0); //AAA reverse?! YES
 //__m128d t3 = _mm_setr_pd(3.0, 10.0); //AAA reverse?! YES
 //_mm_setr_pd
 //Function core::arch::x86_64::_mm_setr_pdCopy item path
//1.27.0 · source · [−]
//pub unsafe fn _mm_setr_pd(a: f64, b: f64) -> __m128d
//Available on 
//(x86 or x86-64) and target feature sse2 and x86-64
 //only.
//Sets packed double-precision (64-bit) floating-point elements in the return value //with the supplied values in reverse order.
 
 //__m128d, __m128i, __m256i*, ...  _mm_empty  - void  
 //packed (p),extended packed (ep), or scalar (s)/
 //__m128d sum = _mm_add_ss(t2 , t3); mmx?
 //__m128d sum = _mm128_add_pd(t2,t3);
 __m128d sum = _mm_add_pd(t2,t3);
 p128d_num(sum);
 sum = _mm_add_pd(sum,t2);
 p128d_num(sum);
 sum = _mm_add_pd(sum,t2);
 p128d_num(sum);
 float s1arr[] = {1.0, 2.0, 3., 4., 5., 6., 7., 8.};
 float s2arr[] = {5.0, 6.3, 3.33, 9.35, 0.56, 1.445, 7.31, 8.4566};
 __m256 s2 = _mm256_load_ps((const float*)s1arr);
 __m256 s3 = _mm256_load_ps((const float*)s2arr);
 //__m256 s2 = _mm_set_ps(1.0, 2.0, 3., 4., 5., 6., 7., 8.);
 //__m256 s3 = _mm_set_ps(9.0, 10.0, 11., 12., 13., 14., 15., 16.); 
 __m256 sums = _mm256_add_ps(s2,s3);
 print256(sums);
 
//MAIN NO: setr_pd --> reverse order
//DIAGONAL SUM! (1+2), (1+5) = 3, 6
// 3,6 + (1,2) = ?
//#v2_u64: 3.000000 6.000000
//#v2_u64: 5.000000 7.000000

//#1 2
//#1 5 
//#\/
//#/\

//#3 6
//#1 2

//#5 7

}

int main(){
	printf("MAIN");
  all();
}

/*
  unsigned char* data_ptr = new unsigned char[(size_t)SIZE];
  //unsigned char* indexes_ptr = new unsigned char[(size_t)LEN];
  unsigned int* indexes_ptr = new unsigned int[(size_t)LEN];
  unsigned char* zeros = new unsigned char[64];
  memset(zeros, 0, sizeof(zeros));
  memset(data_ptr, 0, SIZE); //sizeof(data_ptr));
  for(int i=0; i< 10000; i++) data_ptr[i]=1;

  //for(unsigned int i=0; i<len*8; i++) indexes_ptr[i]=(unsigned int)(i%4);
  for(unsigned int i=0; i<len*8; i++) indexes_ptr[i]=(unsigned int)(i);
  for(unsigned i=0; i<len*8; i++){
	  printf("%u: %u\n", i, indexes_ptr[i]);
  }
  //for(unsigned int i=0; i<len; i++) indexes_ptr[i]=0; //(unsigned char)(i%4);
  
  __m256i sum = _mm256_loadu_si256( (const __m256i_u*) zeros);
  	printf("__m256i sum OK\n");
  for (int i = 0; i < len; i+=8) {	
    const __m256i* indexes_2 = reinterpret_cast<const __m256i*>(indexes_ptr + i);
	printf("#1");	
    __m256i index_reg = _mm256_loadu_si256(indexes_2);
	printf("#index_reg\n");
	print256_num(index_reg);	
    __m256i values = _mm256_i32gather_epi32(data_ptr, index_reg, 4);
	printf("#values\n");
	print256_num(values);
	printf("#3");
    sum = _mm256_add_epi32(sum, values);
	printf("#4");	
	//print_mm256(sum);
	print256_num(sum);
  }
  
  printf("Guz");
  delete(data_ptr);  
  return 0;
}
*/


/*
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

#ifndef __cplusplus
#include <stdalign.h>   // C11 defines _Alignas().  This header defines alignas()
#endif
*/

void p128_hex_u8(__m128i in) {
    alignas(16) uint8_t v[16];
    _mm_store_si128((__m128i*)v, in);
    printf("v16_u8: %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x %x\n",
           v[0], v[1],  v[2],  v[3],  v[4],  v[5],  v[6],  v[7],
           v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
}

void p128_hex_u16(__m128i in) {
    alignas(16) uint16_t v[8];
    _mm_store_si128((__m128i*)v, in);
    printf("v8_u16: %x %x %x %x,  %x %x %x %x\n", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

void p128_hex_u32(__m128i in) {
    alignas(16) uint32_t v[4];
    _mm_store_si128((__m128i*)v, in);
    printf("v4_u32: %x %x %x %x\n", v[0], v[1], v[2], v[3]);
}

void p128_hex_u64(__m128i in) {
    alignas(16) unsigned long long v[2];  // uint64_t might give format-string warnings with %llx; it's just long in some ABIs
    _mm_store_si128((__m128i*)v, in);
    printf("v2_u64: %llx %llx\n", v[0], v[1]);
}



/*
https://stackoverflow.com/questions/72772188/memory-throughput-for-strided-memory-accesses
https://stackoverflow.com/questions/19470873/why-does-gcc-generate-15-20-faster-code-if-i-optimize-for-size-instead-of-speed?rq=1
https://www.intel.com/content/www/us/en/develop/documentation/cpp-compiler-developer-guide-and-reference/top/compiler-reference/intrinsics/intrinsics-for-intel-advanced-vector-extensions/intrinsics-for-load-and-store-operations-1/mm256-loadu-si256.html
https://www.google.com/search?q=undefined+reference+to+%60operator+new%5B%5D&oq=undefined+reference+to+%60operator+new%5B%5D&aqs=chrome..69i57j0i512l2j0i22i30l7.474j0j7&sourceid=chrome&ie=UTF-8
https://stackoverflow.com/questions/3735804/undefined-reference-to-operator-new
https://stackoverflow.com/questions/2788388/when-is-include-new-library-required-in-c
https://www.google.com/search?q=gcc+undefined+reference+to+operator+new&oq=gcc+undefined+reference+to+operator&aqs=chrome.1.69i57j0i22i30l4.8272j0j7&sourceid=chrome&ie=UTF-8
https://forums.freebsd.org/threads/gcc-on-cpp-file-gives-undefined-reference-to-operator-new.52388/
https://www.google.com/search?q=gcc+compile+and+run+C%2B%2B&oq=gcc+compile+and+run+C%2B%2B&aqs=chrome.0.69i59j0i390l4.2081j0j7&sourceid=chrome&ie=UTF-8
https://stackoverflow.com/questions/3178342/compiling-a-c-program-with-gcc
https://www.washington.edu/doit/technology-tips-chmod-overview#:~:text=To%20give%20the%20owner%20all,type%20chmod%20705%20%5Bfilename%5D.
https://www.google.com/search?q=g%2B%2B+compile+and+run&oq=g%2B%2B+compile+and+run&aqs=chrome..69i57j0i512l2j0i22i30l2j0i15i22i30j0i390l3.4681j0j7&sourceid=chrome&ie=UTF-8
https://www.geeksforgeeks.org/compiling-with-g-plus-plus/
https://learn.microsoft.com/en-us/dotnet/api/system.runtime.intrinsics.x86.avx2.gathervector256?view=netcore-3.0
https://www.intel.com/content/www/us/en/develop/documentation/cpp-compiler-developer-guide-and-reference/top/compiler-reference/intrinsics/intrinsics-for-avx2/intrinsics-for-gather-operations/mm-i32gather-epi32-mm256-i32gather-epi32.html
https://stackoverflow.com/questions/71142195/can-i-speed-up-more-than-mm256-i32gather-epi32
https://www.google.com/search?q=mm256_i32gather_epi32+&sxsrf=ALiCzsahk-vB0orx0kARHWKBuCiI9VJmDQ%3A1666478559454&ei=33FUY4GdG86Dxc8PpZ6ViAw&ved=0ahUKEwjBiL-09PT6AhXOQfEDHSVPBcEQ4dUDCA8&uact=5&oq=mm256_i32gather_epi32+&gs_lcp=Cgdnd3Mtd2l6EAMyBAgjECcyBAgAEB4yCAgAEAgQHhAKSgQIQRgBSgQIRhgAUN0HWN0HYLUJaAJwAHgAgAFkiAFkkgEDMC4xmAEAoAEBwAEB&sclient=gws-wiz
https://docs.unity3d.com/Packages/com.unity.burst@1.6/api/Unity.Burst.Intrinsics.X86.Avx2.mm256_mask_i32gather_epi32.html
https://www.intel.com/content/www/us/en/develop/documentation/cpp-compiler-developer-guide-and-reference/top/compiler-reference/intrinsics/intrinsics-for-avx2/intrinsics-for-gather-operations/mm-i32gather-epi32-mm256-i32gather-epi32.html
https://www.google.com/search?q=print+mm256&oq=print+mm256&aqs=chrome..69i57j0i390l3.1511j0j7&sourceid=chrome&ie=UTF-8
https://stackoverflow.com/questions/13257166/print-a-m128i-variable
https://gcc.godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSBnVAV2OUxAHIBSAJgGY8AO2QAbZlgDU3fgGE8AWwXCCxYQDoEM7NwAMAQT6CR4qTNkMC%2BIQU3a9hgcLETM0uZfyo7/HQYdGeABmQlhBkgD6EcgADuIMccwMAU4mru4WVgCGonjAQj7YksUA9CWSsrS0kmHCmAyR%2Brn5WQwQAJTqxQAqCHgNCJhZWMQ1mEF1DTl5Qq0dKfyYocEBBgBuqHjokjG0vAAcEYMAHhHM%2BxBRCnv7eJLC7dIA7ABCDsXF0y1ttABsj8wVIcCJI1twAKwvP4QgAiMjeBg%2BkSUEUsJEwqLwNygVxueAAVO01qR7kJ2vD3h8YmobEEIHxeGs/md9iBpLxwUZjuzOQJuXxefx%2BRyuc9ZDzRQLJSK%2BRLZdwnuKpfKZUK5WrlRrFerhYLddK9RDZEIGaRKUiLWDIbpYSSrVDbcV7bxHaCIS9%2BK77QAWL3uzngmEkt2Q35%2ByFPW3mi0fe37R32gCcCfdtBtgbtqdoKchexzUM9GZDUN9RfttADMPJ/ARhiecP8602212ByOmFOzD%2Blwi1wOdwez1rSK%2Bsx%2B/0kgJszJBcdhFMRH17ClRRGIGIYWIOON7eMJxNJ1eHVJpBDpDLWhy7vzZmv1Kr1OuKCqVqvvGrfOqNJt4vDNi6dd10yDYtszLd0XXAyFCxAn180rTNQ3zSNAyPAJ61WfQNi2HYbnbTt%2BF4Hs%2B1uQ8h2jUc5j%2BAEVEIiJZ3dUs4RraNl1XdFMWxYi9yJEkHgXfQkWpFRz1/NZvTOQjb0/O8nzvb9TWLYDEIdKCXkg2D3RgtD/AwxssObXC2xOM5fm9bj%2BzIhVj0%2BZoxwgajJFEVAhGAJyXLc515xrUpyinAgzPoyRlGABAQWAPA1jcIISAULICAAWksGk3IAdyyYghGEYAGlSwgEB5URRGOeF7gIMBOAaKgkhBZzXNJSQmAUNx9BeABJZIAORFc0XXTjtws259z4skBKE09RMZXhTO9aTBSK98FoU38VOU0DYR0usG0MfTAnCG0DBUYKsmECBsPQR5rMpXFLKi5B3BhbrUUwVQIkwGI8AuehJD/SR%2BBJb0SXBElfhJJ5/0EmMoehj59hJRMSTTRHvr2RH/skWhAYx4GMf%2BMbilbQ4TOYMyzswZBNqpPDicIsmKfxoyiY7M5uzuymCep5nzjpzaFW2owlnwIJOHaUhRC4cFOFIIQuF0KXUC4WQ%2BDeXgXkalg2DcIxaClghZZF0WAGsQH4cF1ETRNvXBRNCN/X5TaeUHxc4b0pYUOhdF0aX9dIBXOClhgQC9vXODl0W4FgGBEBQVAFA%2B0RMDICgIDQOO8AT4gQDwZBkFocGJlEAhE8DiAACMfdL4QMoATy4HXSFT5qbAAeSEURa9DqWsDi1yE59/B12QAhIvqH2O3J5gi7rqWVEwZ25dIVRFGn0XclLwPIFF1AYmHlzA84BLjnuhKAHUclESQEub/hL%2BOAAJZuAGVugDjX2DoVeJalmXO99rhjn2L8BKZl7g5wxk8SQEBcCEBIOyfg31ZCx3jonOBvB2i631u0I2IBeDenNpbcE/ALYEMBs7V2pB3ben2OoX4vBfi/EIWmXgTxvT0Mtt7X%2BfsA5B0Xpg0gEdo6p2QUnSgQj06JxQKILIrlwSe1IAXIuxAS7l1/pXWYxAO710bksAgrd2790wD3YAfdf4D3JsPKK%2B8F7j2QJPDgnB66z3nrrNQ7sHEizFngde8At47zwHvLgSUrAyBhPkZgytlZVEvmfIql9r6XzimwLQ/AYQIFaKlOeohX6sHfrQT%2BnBJYcIXn7ABQCQFiGkW5M2uh1C6EgdAtccCEFIPEaMbW6DeGdywaQQYwwJEdDFlwch7twS0HUPwJ4/B%2BCsP2ImJ4HIplFPllwbhwdMHYP4DU8ETwni6H2Psm2NtdATMGZwfg38fZcM6WHfhUcBEQCQGIjO5BREtIzpI2giYIhmXkenRRyiK5Vw0dPBuscm66Lbh3Be3dKkmOhXgQeFjR6/xsXYkFTifZLzcTrVeXiN4DO3rvIQ%2B8r7HGyZrD%2BpzCk/2Kf/QBwDvSSAALKPwAGrik%2BT9XQedJAACVugn3qfgRp2sSSILThnOB/AOkhzDhss2UzEx/H2N6RM3LGHgzIW7EATxqGW14Ny70uDeBVGtuDGlyz/aMB4bKjx9ykAsAIDESeLyU5vIkf9TAwqSCUudtSy5XAjA3zygQAqpSGXMrZRyxMXKeX8pPhgrpotekjEoPk4ZOCxm6G2YmfY2ztn7O9LoAQSy/5WsDmspNpBjZTPUEa/Y8DdUNu9FM/Zpzzmlquba7pzteAXM4Ss65BtSBRSUf4mW3ogA%3D%3D

*/
