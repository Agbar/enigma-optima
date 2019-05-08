#pragma once

#include <emmintrin.h>

#include "character_encoding.h"
#include "config/types.h"
#include "dict.h"
#include "score.h"

static inline
void Unpack_v16qu( v16qu in, v8hu *lo, v8hu *hi ){
    __m128i zero = {};
    *lo = (v8hu) _mm_unpacklo_epi8( (__m128i)in, zero );
    *hi = (v8hu) _mm_unpackhi_epi8( (__m128i)in, zero );
}

__attribute__ ((optimize("unroll-loops")))
static inline
int staticComputeTriscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len ){
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

        aLo += bLo + cLo;
        aHi += bHi + cHi;

        int j;
        for( j=0; j< 8; j++ ){
            score += *( ( dict_t* ) tridict + aLo[j] );
            score += *( ( dict_t* ) tridict + aHi[j] );
        }
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


__attribute__ ((optimize("unroll-loops")))
static inline
int staticComputeBiscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0;
    int i;
    for( i = 0; i * 16 + 15 < len - 1; ++i ) {
        v16qu a = v16_echar_0_based_index( msg->vector16[i] );
        v8hu aLo, aHi;
        Unpack_v16qu( a, &aLo, &aHi );
        aLo *= 32;
        aHi *= 32;
        const void* a_addr = &msg->vector16[i];
        const union v16_echar b_vector = { .vector = (v16qs)_mm_loadu_si128( a_addr + 1 ) };
        const v16qu b = v16_echar_0_based_index( b_vector );
        v8hu bLo, bHi;
        Unpack_v16qu( b, &bLo, &bHi );

        aLo += bLo;
        aHi += bHi;

        int j;
        for( j=0; j< 8; j++ ){
            score += *( ( dict_t* ) bidict + aLo[j] );
            score += *( ( dict_t* ) bidict + aHi[j] );
        }
    }
    i *= 16;
    uint_fast8_t c0 = echar_0_based_index( msg->plain[i] );
    int j = i + 1;
    for( ; j + 3 < len; j += 4 ) {
        uint_fast8_t j0 = echar_0_based_index( msg->plain[j] );
        score += bidict[ c0 ][ j0 ];
        uint_fast8_t j1 = echar_0_based_index( msg->plain[j + 1] );
        score += bidict[ j0 ][ j1 ];
        uint_fast8_t j2 = echar_0_based_index( msg->plain[j + 2] );
        score += bidict[ j1 ][ j2 ];
        uint_fast8_t j3 = echar_0_based_index( msg->plain[j + 3] );
        score += bidict[ j2 ][ j3 ];
        c0 = j3;
    }    
    for( ; j < len; ++j ) {
        uint_fast8_t j0 = echar_0_based_index( msg->plain[j] );
        score += bidict[ c0 ][ j0 ];
        c0 = j0;
    }
    return score;
}

