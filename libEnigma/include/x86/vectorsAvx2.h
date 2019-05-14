#pragma once

#include <immintrin.h>
#include <stdint.h>

static inline
__m256i m256_setmask_epi32( uint8_t bit_mask ){
     const __m256i mask_v = _mm256_set1_epi32( bit_mask );
     const __m256i shifts_to_highest = _mm256_set_epi32( 31-7, 31-6, 31-5, 31-4, 31-3, 31-2, 31-1, 31-0 );
     const __m256i shifted = _mm256_sllv_epi32( mask_v, shifts_to_highest );
     return shifted;
}