#pragma once

#include <immintrin.h>

#include "echar.h"
#include "global.h"
#include "key.h"

CONST_FUNCTION
static inline
v32qs PermuteV32qs(const union PermutationMap_t map, v32qs vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x3F;
    vec += ( char ) 0x70; // For every byte push value of bit[4] to bit[7].
    vec ^= ( v32qs ) _mm256_set_epi32( 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0, 0, 0, 0 );
    __m256i pMap = (__m256i) map.whole.vector;
    __m256i mVec = (__m256i) vec;
    __m256i ret1 = _mm256_shuffle_epi8( pMap, mVec );
            pMap = _mm256_permute2x128_si256( pMap, pMap , 1 ); //VPERM2I128
    __m256i ret2 = _mm256_shuffle_epi8( pMap , mVec ^ _mm256_set1_epi8( 0x80 ) );
    return (v32qs) _mm256_or_si256( ret1, ret2 );
}

CONST_FUNCTION
static inline
union v32_echar
v32_echar_map( union v32_echar c, const union PermutationMap_t m ){
    v32qs r = PermuteV32qs( m, c.vector );
    return (union v32_echar){ .vector = r };
}