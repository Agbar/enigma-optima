#pragma once

#include <tmmintrin.h>

#include "config/types.h"
#include "score.h"

static inline
void Unpack_v16qu( v16qu in, v8hu *lo, v8hu *hi ){
    __m128i zero = {};
    *lo = (v8hu) _mm_unpacklo_epi8( (__m128i)in, zero );
    *hi = (v8hu) _mm_unpackhi_epi8( (__m128i)in, zero );
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
uint16_t staticComputeIcscoreFromDecodedMsgSsse3( union ScoringDecodedMessage* msg, scoreLength_t len ){
    ALIGNED_32( uint8_t f[32] ) = {};
    int i;
    for( i = 0; i < len; i++ ) {
        f[ echar_0_based_index( msg->plain[i] ) ]++;
    }
    v16qu* const v = ( v16qu* ) f; // it makes v16hi[2];
    // short result[i] = v0[2*i] * ( v0[2*i] + minusOne ) + v0[2*i+1] * ( v0[2*i+1] + minusOne );
    // multiply-add sign-saturated
    v8hu foo = (v8hu)_mm_maddubs_epi16( (__m128i)v[0], (__m128i) (v[0] + -1 ) );
    v8hu bar = (v8hu)_mm_maddubs_epi16( (__m128i)v[1], (__m128i) (v[1] + -1 ) );
    // pmaddubsw may overflow and saturate for 182 and higher
    if( len > 181 ) {
        __m128i sat_max = _mm_srli_epi16( _mm_set1_epi16( ~0 ), 1 );
        __m128i foo_sat = _mm_cmpeq_epi16( (__m128i)foo, sat_max );
        __m128i bar_sat = _mm_cmpeq_epi16( (__m128i)bar, sat_max );
        uint32_t any_sat = _mm_movemask_epi8( foo_sat | bar_sat );
        if( any_sat ) {
            v8hu lo, hi;
            Unpack_v16qu( v[0], &lo, &hi );
            foo = lo * ( lo + -1 ) + hi * (hi + -1);
            Unpack_v16qu( v[1], &lo, &hi );
            bar = lo * ( lo + -1 ) + hi * (hi + -1);
        }
    }
    foo += bar;

    __m128i s8 =  (__m128i)foo;
    __m128i s8h = _mm_shuffle_epi32( s8, 0b1110 );
    __m128i s4  = _mm_add_epi16( s8, s8h );
    __m128i s4h = _mm_shuffle_epi32( s4, 0b01 );
    __m128i s2  = _mm_add_epi16( s4, s4h );
    union
    {
        uint32_t epu32;
        uint16_t epu16[2];
    } s2x = {.epu32 = _mm_cvtsi128_si32( s2 )};
    return s2x.epu16[0] + s2x.epu16[1];
}
