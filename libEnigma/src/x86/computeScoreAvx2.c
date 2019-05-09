#pragma GCC target ("avx2")

#include <immintrin.h>

#include "config/types.h"
#include "dict.h"
#include "x86/computeScoreAvx2.h"

static inline
void Unpack_v16qu( v16qu in, v8hu *lo, v8hu *hi ){
    __m128i zero = {};
    *lo = (v8hu) _mm_unpacklo_epi8( (__m128i)in, zero );
    *hi = (v8hu) _mm_unpackhi_epi8( (__m128i)in, zero );
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((hot))
uint32_t ComputeTriscoreFromDecodedMsgAvx2( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    int score = 0;
    int i;
    for( i = 0; i * 16 + 15 < len - 2; ++i ) {
        v16qu a = v16_echar_0_based_index( msg->vector16[i] );
        v8hu aLo, aHi;
        Unpack_v16qu( a, &aLo, &aHi );
        aLo *= 32 * 32;
        aHi *= 32 * 32;
        const void* a_addr = &msg->vector16[i].vector ;
        union v16_echar b_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 1 ) };
        v16qu b = v16_echar_0_based_index( b_vector );
        v8hu bLo, bHi;
        Unpack_v16qu( b, &bLo, &bHi );
        bLo *= 32;
        bHi *= 32;
        union v16_echar c_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 2 ) };
        v16qu c = v16_echar_0_based_index( c_vector );
        v8hu cLo, cHi;
        Unpack_v16qu( c, &cLo, &cHi );

        v8hu loIdx = aLo + bLo + cLo;
        v8hu hiIdx = aHi + bHi + cHi;

        const __m128i tridx0 = _mm_cvtepu16_epi32( (__m128i) loIdx );
        v4su score0 = (v4su)_mm_i32gather_epi32( (const int*)tridict, tridx0, 4 );
        const __m128i tridx1 = _mm_cvtepu16_epi32( _mm_srli_si128( (__m128i) loIdx, 8 ) );
        v4su score1 = (v4su)_mm_i32gather_epi32( (const int*)tridict, tridx1, 4);
        const __m128i tridx2 = _mm_cvtepu16_epi32( (__m128i) hiIdx );
        v4su score2 = (v4su)_mm_i32gather_epi32( (const int*)tridict, tridx2, 4 );
        const __m128i tridx3 = _mm_cvtepu16_epi32( _mm_srli_si128( (__m128i) hiIdx, 8 ) );
        v4su score3   = (v4su)_mm_i32gather_epi32( (const int*)tridict, tridx3, 4);

        v4su score01   = score0 + score1;
        v4su score23   = score2 + score3;
        v4su score0123 = score01 + score23;

        __m128i s1 = _mm_hadd_epi32( (__m128i)score0123, (__m128i)score0123 );
        __m128i s2 = _mm_hadd_epi32( s1, s1 );
        score += _mm_cvtsi128_si32( s2 );
    }
    i *= 16;
    uint_fast8_t c0 = echar_0_based_index( msg->plain[i] );
    uint_fast8_t c1 = echar_0_based_index( msg->plain[i + 1] );
    int k = i + 2;
    for( ; k + 3 < len; k += 4 ) {
        uint_fast8_t k0 = echar_0_based_index( msg->plain[k] );
        score += tridict[ c0 ][ c1 ][ k0 ];
        uint_fast8_t k1 = echar_0_based_index( msg->plain[k + 1] );
        score += tridict[ c1 ][ k0 ][ k1 ];
        uint_fast8_t k2 = echar_0_based_index( msg->plain[k + 2] );
        score += tridict[ k0 ][ k1 ][ k2 ];
        uint_fast8_t k3 = echar_0_based_index( msg->plain[k + 3] );
        score += tridict[ k1 ][ k2 ][ k3 ];
        c0 = k2;
        c1 = k3;
    }
    for( ; k < len; ++k ) {
        uint8_t c2 = echar_0_based_index( msg->plain[k] );
        score += tridict[ c0 ][ c1 ][ c2 ];
        c0 = c1;
        c1 = c2;
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
