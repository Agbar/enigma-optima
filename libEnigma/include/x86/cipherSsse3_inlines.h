#pragma once

#include "error.h"
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
