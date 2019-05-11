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
    for( i = 0; i * 32 + 31 < len - 2; ++i ) {
        v32qu a = v32_echar_0_based_index( msg->vector32[i] );
        const __m128i al128 = _mm256_castsi256_si128( (__m256i)a );
        const __m128i ah128 = _mm256_extracti128_si256( (__m256i)a, 1 );
        __m256i a0 = _mm256_cvtepu8_epi32( al128 );
        __m256i a1 = _mm256_cvtepu8_epi32( _mm_srli_si128( al128 , 8 ) );
        __m256i a2 = _mm256_cvtepu8_epi32( ah128 );
        __m256i a3 = _mm256_cvtepu8_epi32( _mm_srli_si128( ah128 , 8 ) );
        a0 *= 32 * 32;
        a1 *= 32 * 32;
        a2 *= 32 * 32;
        a3 *= 32 * 32;
        const void* a_addr = &msg->vector32[i].vector;
        union v32_echar b_vector = { .vector = (v32qs)_mm256_loadu_si256( a_addr + 1 ) };
        v32qu b = v32_echar_0_based_index( b_vector );
        const __m128i bl128 = _mm256_castsi256_si128( (__m256i)b );
        const __m128i bh128 = _mm256_extracti128_si256( (__m256i)b, 1 );
        __m256i b0 = _mm256_cvtepu8_epi32( bl128 );
        __m256i b1 = _mm256_cvtepu8_epi32( _mm_srli_si128( bl128 , 8 ) );
        __m256i b2 = _mm256_cvtepu8_epi32( bh128 );
        __m256i b3 = _mm256_cvtepu8_epi32( _mm_srli_si128( bh128 , 8 ) );
        b0 = b0 * 32 + a0;
        b1 = b1 * 32 + a1;
        b2 = b2 * 32 + a2;
        b3 = b3 * 32 + a3;
        union v32_echar c_vector = { .vector = (v32qs)_mm256_loadu_si256( a_addr + 2 ) };
        v32qu c = v32_echar_0_based_index( c_vector );
        const __m128i cl128 = _mm256_castsi256_si128( (__m256i)c );
        const __m128i ch128 = _mm256_extracti128_si256( (__m256i)c, 1 );
        __m256i c0 = _mm256_cvtepu8_epi32( cl128 );
        __m256i c1 = _mm256_cvtepu8_epi32( _mm_srli_si128( cl128 , 8 ) );
        __m256i c2 = _mm256_cvtepu8_epi32( ch128 );
        __m256i c3 = _mm256_cvtepu8_epi32( _mm_srli_si128( ch128 , 8 ) );
        c0 += b0;
        c1 += b1;
        c2 += b2;
        c3 += b3;
        v8su score0 = (v8su)_mm256_i32gather_epi32( (const int*)tridict, c0, 4 );
        v8su score1 = (v8su)_mm256_i32gather_epi32( (const int*)tridict, c1, 4 );
        v8su score2 = (v8su)_mm256_i32gather_epi32( (const int*)tridict, c2, 4 );
        v8su score3 = (v8su)_mm256_i32gather_epi32( (const int*)tridict, c3, 4 );

        v8su score01   = score0 + score1;
        v8su score23   = score2 + score3;
        v8su score0123 = score01 + score23;

        __m256i s1 = _mm256_hadd_epi32( (__m256i)score0123, (__m256i)score0123 );
        __m256i s2 = _mm256_hadd_epi32( s1, s1 );
        __m128i sh = _mm256_extracti128_si256(s2,1);
        __m128i sl = _mm256_castsi256_si128(s2);
        __m128i s  = _mm_add_epi32(sh,sl);
                      
        score += _mm_cvtsi128_si64( s );
    }
    i *= 32;
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
