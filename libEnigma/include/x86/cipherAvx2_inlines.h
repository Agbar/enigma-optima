#pragma once

#include "error.h"
#include "cipherAvx2.h"
#include "echar/echar_avx2.h"

static inline
union v32_echar
DecodeBiteForwardCommonAvx2( union v32_echar bite, union v32_echar_delta rRingOffset, const struct Key* const restrict key ) {
    // stbrett forward
    bite = v32_echar_map ( bite, key->stbrett );
    // right ring forward
    bite = v32_echar_add_delta( bite, rRingOffset );
    bite = v32_echar_map( bite, PathLookupAvx2.r_ring[0] );
    bite = v32_echar_sub_delta( bite, rRingOffset );
    return bite;
}

static inline
union v32_echar
DecodeBiteMaskedPartAvx2( union v32_echar predecodedBite, int lookupNumber ) {
    // m+l rings and ukw
    union v32_echar masked = v32_echar_map( predecodedBite, PathLookupAvx2.lookups[lookupNumber].mapping );
    masked.vector &= PathLookupAvx2.lookups[lookupNumber].mask;
    return masked;
}

static inline
union v32_echar
DecodeBiteBackwardCommonAvx2( union v32_echar bite, union v32_echar_delta rRingOffset, const struct Key* const key ) {
    // right ring backwards
    bite = v32_echar_add_delta( bite, rRingOffset );
    bite = v32_echar_map( bite, PathLookupAvx2.r_ring[1] );
    bite = v32_echar_sub_delta( bite, rRingOffset );
    //stbrett backwards
    bite = v32_echar_map( bite, key->stbrett );
    return bite;
}

static inline
union v32_echar
CombineMaskedPartsAvx2( union v32_echar l, union v32_echar r ){
    return (union v32_echar){ .vector = l.vector | r.vector };
}

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
void DecodeScoredMessagePartAvx2( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;
    union v32_echar_delta currentRRingOffset = PathLookupAvx2.firstRRingOffset;
    while( messageBite < ( len + 31 ) / 32 )
    {
        /* Worst case:
         *  P0123456789ABCDEFGHIJKLMNOP01234    R-ring position (turnovers on 12 & 25, coded C & P)
         *  0123456789ABCDEF0123456789ABCDEF    characters in bite
         *  |||           |            |
         *  |||           |            +
         *  |||           +-  turnover on second notch of R ring
         *  ||+- turnover caused by M-ring (turning L- & M- rings).
         *  |+-- turnover setting M-ring to turnover position
         *  +--- last character from previous bite
         *
         *  In the worst case there are 5 lookups per bite.
         */
        uint_least16_t lookupsToNextBite = PathLookupAvx2.nextBite[messageBite] - lookupNumber;
        union v32_echar cBite = {};
        lookupNumber += lookupsToNextBite;
        union v32_echar currentBite = ciphertext.vector32[messageBite];
        union v32_echar predecoded = DecodeBiteForwardCommonAvx2( currentBite, currentRRingOffset, key );

        switch( lookupsToNextBite ) {
        case 5:
            cBite  = DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 5 );
            FALLTHROUGH();
        case 4:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 4 ) );
            FALLTHROUGH();
        case 3:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 3 ) );
            FALLTHROUGH();
        case 2:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 2 ) );
            FALLTHROUGH();
        case 1:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 1 ) );
            break;
        default:
            exit_d(5);
            UNREACHABLE();
        }
        cBite = DecodeBiteBackwardCommonAvx2( cBite, currentRRingOffset, key );
        // store whole decoded bite
        output -> vector32[messageBite] = cBite;
        messageBite++;
        currentRRingOffset = v32_echar_delta_rot_32( currentRRingOffset );
    }
}
