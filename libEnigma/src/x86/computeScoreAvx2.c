#pragma GCC target ("avx2")

#include <immintrin.h>

#include "config/types.h"
#include "dict.h"
#include "x86/computeScoreAvx2.h"
#include "x86/vectorsAvx2.h"

static inline
void Unpack_v16qu( v16qu in, v8hu *lo, v8hu *hi ){
    __m128i zero = {};
    *lo = (v8hu) _mm_unpacklo_epi8( (__m128i)in, zero );
    *hi = (v8hu) _mm_unpackhi_epi8( (__m128i)in, zero );
}

__attribute__ ((hot))
uint32_t ComputeTriscoreFromDecodedMsgAvx2( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    int score = 0;
    int i;
    for( i = 0; i * 16 < len - 2; ++i ) {
        v16qu a = v16_echar_0_based_index( msg->vector16[i] );
        __m256i al = _mm256_cvtepu8_epi32( (__m128i)a );
        __m256i ah = _mm256_cvtepu8_epi32( _mm_srli_si128( (__m128i)a , 8 ) );
        al *= 32 * 32;
        ah *= 32 * 32;
        const void* a_addr = &msg->vector16[i].vector;
        union v16_echar b_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 1 ) };
        v16qu b = v16_echar_0_based_index( b_vector );
        __m256i bl = _mm256_cvtepu8_epi32( (__m128i)b );
        __m256i bh = _mm256_cvtepu8_epi32( _mm_srli_si128( (__m128i)b , 8 ) );
        bl = bl * 32 + al;
        bh = bh * 32 + ah;
        union v16_echar c_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 2 ) };
        v16qu c = v16_echar_0_based_index( c_vector );
        __m256i cl = _mm256_cvtepu8_epi32( (__m128i)c );
        __m256i ch = _mm256_cvtepu8_epi32( _mm_srli_si128( (__m128i)c , 8 ) );
        cl += bl;
        ch += bh;
        __m256i z = _mm256_setzero_si256();
        __m256i mask0 = _mm256_set1_epi32(~0);
        __m256i mask1 = _mm256_set1_epi32(~0);
        int tail =  len - 2 - 16 * i;
        if( tail < 16 ) {
            uint16_t bit_mask = 0xFFFF >> ( 16 - tail );
            mask0 = m256_setmask_epi32( bit_mask );
            mask1 = m256_setmask_epi32( bit_mask >> 8 );
        }
        v8su score0 = (v8su)_mm256_mask_i32gather_epi32( z, (const int*)tridict, cl, mask0, 4 );
        v8su score1 = (v8su)_mm256_mask_i32gather_epi32( z, (const int*)tridict, ch, mask1, 4 );
        v8su score01   = score0 + score1;

        __m256i s1 = _mm256_hadd_epi32( (__m256i)score01, (__m256i)score01 );
        __m256i s2 = _mm256_hadd_epi32( s1, s1 );
        __m128i sh = _mm256_extracti128_si256( s2, 1 );
        __m128i sl = _mm256_castsi256_si128( s2 );
        __m128i s  = _mm_add_epi32( sh, sl );

        score += _mm_cvtsi128_si32( s );
    }
    return score;
}

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
    __m256i hexFF00 = _mm256_set1_epi16( 0xFF00 );
    __m256i bar = _mm256_andnot_si256( hexFF00, (__m256i)foo );
            foo = (v16hu)_mm256_and_si256( hexFF00, (__m256i)foo );

    __m256i high_sum =_mm256_sad_epu8 ( (__m256i)foo, zero );
    v16hu high = (v16hu)_mm256_slli_si256( high_sum, 1 ); // * 256
    v16hu low  = (v16hu)_mm256_sad_epu8(  bar, zero );
    v16hu vSum = high + low;
    uint16_t sum = vSum[0] + vSum[4] + vSum[8] + vSum[12];

    return sum;
}
