#pragma once

#include <tmmintrin.h>

#include "echar.h"
#include "global.h"
#include "key.h"

CONST_FUNCTION
static inline
v16qs PermuteV16qs(const union PermutationMap_t map, v16qs vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x1F;
    vec += (char) 0x70; // For every byte push value of bit[4] to bit[7].
    __m128i mVec = (__m128i) vec;
    __m128i ret1 = _mm_shuffle_epi8( (__m128i)map.half[0].vector, mVec );
    __m128i ret2 = _mm_shuffle_epi8( (__m128i)map.half[1].vector, mVec ^ _mm_set1_epi8( 0x80 ) );
    return (v16qs) (ret1 | ret2);
}

CONST_FUNCTION
static inline
union v16_echar
v16_echar_map( union v16_echar c, const union PermutationMap_t m ){
    v16qs r = PermuteV16qs( m, c.vector );
    return (union v16_echar){ .vector = r };
}