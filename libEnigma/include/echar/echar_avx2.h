#pragma once

#include <immintrin.h>

#include "echar.h"
#include "global.h"
#include "key.h"

CONST_FUNCTION
static inline
v32qi PermuteV32qi(const union PermutationMap_t map, v32qi vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x3F;
    vec += ( char ) 0x70; // For every byte push value of bit[4] to bit[7].
    vec ^= ( v32qi ) _mm256_set_epi32( 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0, 0, 0, 0 );
    v32qi pMap = map.whole.vector;
    v32qi ret1 = __builtin_ia32_pshufb256( pMap, vec );
    pMap = ( v32qi ) __builtin_ia32_permti256( ( v4di )pMap ,( v4di )pMap , 1 ); //VPERM2I128
    v32qi ret2 = __builtin_ia32_pshufb256( pMap , vec ^ (char) 0x80 );
    return ret1 | ret2;
}

CONST_FUNCTION
static inline
union v32_echar
v32_echar_map( union v32_echar c, const union PermutationMap_t m ){
    v32qi r = PermuteV32qi( m, c.vector );
    return (union v32_echar){ .vector = r };
}