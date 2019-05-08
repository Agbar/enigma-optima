#pragma once

#include <tmmintrin.h>

#include "error.h"
#include "dict.h"
#include "cipherSsse3.h"
#include "character_encoding.h"

#include "echar/echar_ssse3.h"

static inline
union v16_echar
DecodeBiteForwardCommonSsse3( union v16_echar bite, union v16_echar_delta rRingOffset, const struct Key* const restrict key ){
    // stbrett forward
    bite = v16_echar_map( bite, key->stbrett );
    // right ring forward
    bite = v16_echar_add_delta( bite, rRingOffset );
    bite = v16_echar_map( bite, PathLookupSsse3.r_ring[0] );
    bite = v16_echar_sub_delta( bite, rRingOffset );
    return bite;
}

static inline
union v16_echar
DecodeBiteMaskedPartSsse3( union v16_echar predecodedBite, int lookupNumber ) {    
    // m+l rings and ukw
    union v16_echar masked = v16_echar_map( predecodedBite, PathLookupSsse3.lookups[lookupNumber].mapping );
    masked.vector &= PathLookupSsse3.lookups[lookupNumber].mask;
    return masked;
}

static inline
union v16_echar
DecodeBiteBackwardCommonSsse3( union v16_echar bite, union v16_echar_delta rRingOffset, const struct Key* const restrict key ) {
    // right ring backwards
    bite = v16_echar_add_delta( bite, rRingOffset );
    bite = v16_echar_map( bite, PathLookupSsse3.r_ring[1] );
    bite = v16_echar_sub_delta( bite, rRingOffset );
    //stbrett backwards
    bite = v16_echar_map( bite, key->stbrett );
    return bite;
}

static inline
union v16_echar
CombineMaskedPartsSsse3( union v16_echar l, union v16_echar r ){
    return (union v16_echar){ .vector = l.vector | r.vector };
}

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
void DecodeScoredMessagePartSsse3( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;
    union v16_echar_delta currentRRingOffset = PathLookupSsse3.firstRRingOffset;
    while( messageBite < ( len + 15 ) / 16 )
    {
        /* Worst case:
         *  P0123456789ABCDE    R-ring position (turnovers on 12 & 25, coded C & P)
         *  0123456789ABCDEF    characters in bite
         *  |||           |
         *  |||           +-  turnover on second notch of R ring
         *  ||+- turnover caused by M-ring (turning L- & M- rings).
         *  |+-- turnover setting M-ring to turnover position
         *  +--- last character from previous bite
         *
         *  In the worst case there are 4 lookups per bite.
         */
        uint_least16_t lookupsToNextBite = PathLookupSsse3.nextBite[messageBite] - lookupNumber;
        union v16_echar cBite = {};
        lookupNumber += lookupsToNextBite;
        union v16_echar currentBite = ciphertext.vector16[messageBite];
        union v16_echar predecoded  = DecodeBiteForwardCommonSsse3( currentBite, currentRRingOffset, key );

        switch( lookupsToNextBite ) {
        case 4:
            cBite  = DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 4 );
            FALLTHROUGH();
        case 3:
            cBite = CombineMaskedPartsSsse3( cBite, DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 3 ) );
            FALLTHROUGH();
        case 2:
            cBite = CombineMaskedPartsSsse3( cBite, DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 2 ) );
            FALLTHROUGH();
        case 1:
            cBite = CombineMaskedPartsSsse3( cBite, DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 1 ) );
            break;
        default:
            exit_d(5);
            UNREACHABLE();
        }
        cBite = DecodeBiteBackwardCommonSsse3( cBite, currentRRingOffset, key );
        // store whole decoded bite
        output -> vector16[messageBite] = cBite;
        messageBite++;
        currentRRingOffset = v16_echar_delta_rot_16( currentRRingOffset );
    }
}

static inline
void Unpack_v16qu( v16qu in, v8hu *lo, v8hu *hi ){
    __m128i zero = {};
    *lo = (v8hu) _mm_unpacklo_epi8( (__m128i)in, zero );
    *hi = (v8hu) _mm_unpackhi_epi8( (__m128i)in, zero );
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
uint16_t ComputeIcscoreFromDecodedMsgSsse3( union ScoringDecodedMessage* msg, scoreLength_t len ){
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


