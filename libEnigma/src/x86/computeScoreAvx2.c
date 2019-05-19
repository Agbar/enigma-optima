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
    for( i = 0; i * 16 + 8 < len - 2; ++i ) {
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
        __m128i s2 = _mm256_extracti128_si256( (__m256i)score01, 1 );
        __m128i s3 = _mm256_castsi256_si128( (__m256i)score01 );
        __m128i s23 = _mm_add_epi32( s2, s3 );
        __m128i s23h =_mm_shuffle_epi32( s23, 0b1110 );
        __m128i s4 = _mm_add_epi32( s23, s23h );
        __m128i s4h = _mm_shuffle_epi32( s4, 0b01 );
        __m128i s  = _mm_add_epi32( s4, s4h );

        score += _mm_cvtsi128_si32( s );
    }
    int tail8 =  len - 2 - 16 * i;
    if( tail8 > 0 ) {
         v16qu a = v16_echar_0_based_index( msg->vector16[i] );
        __m256i al = _mm256_cvtepu8_epi32( (__m128i)a );
        al *= 32 * 32;
        const void* a_addr = &msg->vector16[i].vector;
        union v16_echar b_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 1 ) };
        v16qu b = v16_echar_0_based_index( b_vector );
        __m256i bl = _mm256_cvtepu8_epi32( (__m128i)b );
        bl = bl * 32 + al;
        union v16_echar c_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 2 ) };
        v16qu c = v16_echar_0_based_index( c_vector );
        __m256i cl = _mm256_cvtepu8_epi32( (__m128i)c );
        cl += bl;
        uint16_t bit_mask = 0xFFFF >> ( 16 - tail8 );
        __m256i mask0 = m256_setmask_epi32( bit_mask );
        __m256i score0 = _mm256_mask_i32gather_epi32( _mm256_setzero_si256(), (const int*)tridict, cl, mask0, 4 );

        __m128i s2 = _mm256_extracti128_si256( (__m256i)score0, 1 );
        __m128i s3 = _mm256_castsi256_si128( (__m256i)score0 );
        __m128i s23 = _mm_add_epi32( s2, s3 );
        __m128i s23h =_mm_shuffle_epi32( s23, 0b1110 );
        __m128i s4 = _mm_add_epi32( s23, s23h );
        __m128i s4h = _mm_shuffle_epi32( s4, 0b01 );
        __m128i s  = _mm_add_epi32( s4, s4h );

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
