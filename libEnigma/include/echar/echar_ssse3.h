#pragma once

#include "echar.h"
#include "global.h"
#include "key.h"

CONST_FUNCTION
static inline
v16qi PermuteV16qi(const union PermutationMap_t map, v16qi vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x1F;
    vec += (char) 0x70; // For every byte push value of bit[4] to bit[7].
    v16qi ret1 = __builtin_ia32_pshufb128( map.half[0].vector, vec );
    v16qi ret2 = __builtin_ia32_pshufb128( map.half[1].vector, vec ^ (char) 0x80 );
    return ret1 | ret2;
}

CONST_FUNCTION
static inline
union v16_echar
v16_echar_map( union v16_echar c, const union PermutationMap_t m ){
    v16qi r = PermuteV16qi( m, c.vector );
    return (union v16_echar){ .vector = r };
}