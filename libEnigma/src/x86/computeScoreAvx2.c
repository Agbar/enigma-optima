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

    // short result[i] = v0[2*i] * ( v0[2*i] - 1 ) + v0[2*i+1] * ( v0[2*i+1] - 1 );
    __m256i foo   = _mm256_maddubs_epi16( (__m256i)v, (__m256i)( v + -1 ) );
    
    __m256i hexFF00 = _mm256_set1_epi16( 0xFF00 );
    __m256i bar = _mm256_andnot_si256( hexFF00, foo );
            foo = _mm256_and_si256( hexFF00, foo );
    
    __m256i zero  = {};
    __m256i high_sum =_mm256_sad_epu8 ( foo, zero );
    v16hu high = (v16hu)_mm256_slli_si256( high_sum, 1 ); // * 256
    v16hu low  = (v16hu)_mm256_sad_epu8(  bar, zero );
    v16hu vSum = high + low;
    uint16_t sum = vSum[0] + vSum[4] + vSum[8] + vSum[12];

    return sum;
}
