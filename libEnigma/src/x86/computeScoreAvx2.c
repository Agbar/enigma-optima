#pragma GCC target ("avx2")

#include <immintrin.h>

#include "x86/computeScoreAvx2.h"

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
uint16_t ComputeIcscoreFromDecodedMsgAvx2( union ScoringDecodedMessage* msg, scoreLength_t len ){
    ALIGNED_32( uint8_t f[32] ) = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[ echar_0_based_index( msg->plain[i] ) ]++;
    }

    v32qi v;
    /* Following asm is doing: v = *(v32qi*) f;
     * but it uses 256bit load which is fast in AVX2 processors. */
    asm (
        "VMOVDQA %1, %0" :
        "=x" (v) :
        "m" (f)
    );

    // short result[i] = v0[2*i] * ( v0[2*i] - 1 ) + v0[2*i+1] * ( v0[2*i+1] - 1 );
    v16hi foo   = __builtin_ia32_pmaddubsw256( v, v + -1 );
    v32qi zero  = { 0 };
    v16hi mask  = ( v16hi ) _mm256_set1_epi16( 0xFF00 );
    v16hi bar;
    asm(
        "vpandn %[hi], %[mask], %[lo]\n\t"
        "vpand  %[hi], %[mask], %[hi]\n\t" :
        [hi] "+x" ( foo ), [lo] "=&x" ( bar ) :
        [mask]"x" ( mask )
    );

    v16hi high = __builtin_ia32_psadbw256( ( v32qi ) ( foo ), zero );
    v16hi low  = __builtin_ia32_psadbw256( ( v32qi ) ( bar ), zero );

    uint16_t sum = 256 *( high[0] + high[4] + high[8] + high[12] ) + low[0] + low[4] + low[8] + low[12];
    return sum;
}
