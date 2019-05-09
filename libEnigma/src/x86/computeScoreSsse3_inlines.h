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
    v8hu foo;
    v8hu bar;
    v16qu* const v = ( v16qu* ) f; // it makes v16hi[2];
    // pmaddubsw will overflow and saturate for 182 and higher
    if( len <= 181 ) {
        // short result[i] = v0[2*i] * ( v0[2*i] + minusOne ) + v0[2*i+1] * ( v0[2*i+1] + minusOne );
        // multiply-add sign-saturated
        foo = (v8hu)_mm_maddubs_epi16( (__m128i)v[0], (__m128i) (v[0] + -1 ) );
        bar = (v8hu)_mm_maddubs_epi16( (__m128i)v[1], (__m128i) (v[1] + -1 ) );
    } else {
        v8hu lo, hi;
        Unpack_v16qu( v[0], &lo, &hi );
        foo = lo * ( lo + -1 ) + hi * (hi + -1);
        Unpack_v16qu( v[1], &lo, &hi );
        bar = lo * ( lo + -1 ) + hi * (hi + -1);
    }
    foo += bar;
    __m128i hexFF00 = _mm_set1_epi16( 0xFF00 );

    bar = (v8hu)_mm_andnot_si128( hexFF00, (__m128i)foo );
    foo = (v8hu)_mm_and_si128( hexFF00, (__m128i)foo );

    const __m128i zero = {};
    const __m128i high_sum = _mm_sad_epu8( ( __m128i ) foo, zero );
    const v8hu high =  ( v8hu ) _mm_slli_si128( high_sum, 1 ); // * 256
    const v8hu low  =  ( v8hu ) _mm_sad_epu8( ( __m128i ) bar, zero );
    const v8hu vSum = high + low;
    const uint16_t sum =  vSum[0] + vSum[4];

    return sum;
}
