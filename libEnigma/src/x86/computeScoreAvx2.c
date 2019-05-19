#pragma GCC target ("avx2")

#include <immintrin.h>

#include "config/types.h"
#include "x86/computeScoreAvx2.h"

__attribute__ ((hot))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
uint16_t ComputeIcscoreFromDecodedMsgAvx2( union ScoringDecodedMessage* msg, scoreLength_t len ){
    union {
        ALIGNED_32( uint8_t f[32] );
        __m256i fv;
    } u = {};
    int i;
    for( i = 0; i < len; i++ ) {
        u.f[ echar_0_based_index( msg->plain[i] ) ]++;
    }

    v32qi v = (v32qi) _mm256_load_si256( &u.fv );

    __m256i zero  = {};
    v16hu foo;
    if( len <= 181 ) {
        // short result[i] = v0[2*i] * ( v0[2*i] - 1 ) + v0[2*i+1] * ( v0[2*i+1] - 1 );
        foo = (v16hu)_mm256_maddubs_epi16( (__m256i)v, (__m256i)( v + -1 ) );
    } else {
        v16hu lo = (v16hu)_mm256_unpacklo_epi8( (__m256i)v, zero );
        v16hu hi = (v16hu)_mm256_unpackhi_epi8( (__m256i)v, zero );
        foo = lo * ( lo + -1 ) + hi * (hi + -1);
    }
    __m128i sh = _mm256_extracti128_si256( (__m256i)foo, 1 );
    __m128i sl = _mm256_castsi256_si128( (__m256i)foo );
    __m128i s8 = _mm_add_epi16( sh, sl );
    __m128i s8h = _mm_shuffle_epi32( s8, 0b1110 );
    __m128i s4  = _mm_add_epi16( s8, s8h );
    __m128i s4h = _mm_shuffle_epi32( s4, 0b01 );
    __m128i s2  = _mm_add_epi16( s4, s4h );
    union
    {
        uint32_t epu32;
        uint16_t epu16[2];
    } s2x = {.epu32 = _mm_cvtsi128_si32( s2 )};
    return s2x.epu16[0] + s2x.epu16[1];;
}
