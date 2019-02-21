#pragma GCC target ("avx2")

#ifndef __AVX2__
# error AVX2 not defined
#endif

#include <immintrin.h>
#include <stdbool.h>
#include <string.h>

#include "x86/cipherAvx2.h"
#include "x86/cipherAvx2_inlines.h"
#include "cipher_inlines.h"

void prepare_decoder_lookup_M_H3_avx2( const struct Key *key, int len );
void prepare_decoder_lookup_ALL_avx2( const struct Key *key, int len );

enigma_cipher_function_t enigma_cipher_DecoderLookupAvx2 = { prepare_decoder_lookup_M_H3_avx2, prepare_decoder_lookup_ALL_avx2 };

struct PathLookupAvx2_t PathLookupAvx2;

typedef void CalculatePermutationMap_f( union PermutationMap_t* const restrict map, struct RingsState rings, const struct Key* restrict key );

void CalculateLookupAvx2( int lookupNumber, struct RingsState rings, const struct Key* restrict key, CalculatePermutationMap_f* calculatePermutationMap );
void CalculateMaskAvx2( size_t lookupNumber, int8_t begin, int8_t end );
void CalculateRRingOffetsAvx2( int8_t rOffsetAtFirst );

void CalculateRRingOffetsAvx2( int8_t rOffsetAtFirst ) {
    int k;
    // calculate offsets used for cyclic permutation
    v32qi rOffsets;
    for( k = 0; k < 16; k++ ) {
        rOffsets[k] = rOffsetAtFirst++;
    }
    if( rOffsetAtFirst >= 26 ) rOffsetAtFirst -= 26;
    for ( ; k < 32; k++ ){
        rOffsets[k] = rOffsetAtFirst++;
    }
    rOffsets -= ( rOffsets >= 26 ) & 26;
    PathLookupAvx2.firstRRingOffset.vector = rOffsets;
}

void CalculateLookupAvx2( int lookupNumber, struct RingsState rings, const struct Key* const restrict key, CalculatePermutationMap_f* calculatePermutationMap )
{
    // calculate m,l,(g),u,(g-1),l-1,m-1 mapping
    calculatePermutationMap( &PathLookupAvx2.lookups[lookupNumber].mapping, rings, key );
}

void CalculateMaskAvx2( size_t lookupNumber, int8_t begin, int8_t end )
{
    // erase previous contents
    PathLookupAvx2.lookups[lookupNumber].mask ^= PathLookupAvx2.lookups[lookupNumber].mask;
    int8_t current = begin;
    for( ; current < end; current++ ) {
        PathLookupAvx2.lookups[lookupNumber].mask[current] = ~0;
    }
}

#define SECOND_TURNOVER_POINT 25

inline extern
void PrepareDecoderLookupAvx2( CalculatePermutationMap_f* calculateMap, const struct Key *const restrict key, int len ) {
    CopyRRing2Lookup( key, PathLookupAvx2.r_ring );

    // rings.r will be a position of R-ring for current lookup chunk
    struct RingsState rings = {
        .r = SubMod26( key->mesg.r, key->ring.r ),
        .m = SubMod26( key->mesg.m, key->ring.m ),
        .l = SubMod26( key->mesg.l, key->ring.l ),
        .g = SubMod26( key->mesg.g, key->ring.g ),
    };

    /* calculate turnover points from ring settings */
    struct Turnovers_t turns = {
        .r = SubMod26( wal_turn[key->slot.r.type], key->ring.r ),
        .m = SubMod26( wal_turn[key->slot.m.type], key->ring.m ),
    };

    /* second turnover points for wheels 6,7,8 */
    if( key->slot.r.type > 5 ) {
        turns.r2 = SubMod26( SECOND_TURNOVER_POINT, key->ring.r );
    }
    else {
        turns.r2 = -1;
    }
    if( key->slot.m.type > 5 ) {
        turns.m2 = SubMod26( SECOND_TURNOVER_POINT, key->ring.m );
    }
    else {
        turns.m2 = -1;
    }

    // simulate first key-press and get rid of possible first-press l,m-rings turn
    StepAllRings( &rings, turns );
    int8_t rAtFirstPos = rings.r;       ///< Right ring state _after_ pressing a button.
    int messagePosition = 0;            ///< Index of first character decoded by current lookup.
    int biteCounter  = 0;               ///< Number of bite currently processed.

    CalculateRRingOffetsAvx2( rAtFirstPos );

    int i;
    int bitesLimit = ( len + 31 )/ 32;
    for( i = 0; i < 24; i++ ) //fixme
    {
        CalculateLookupAvx2( i, rings, key, calculateMap );

        // check whether next M-ring turn is within current text bite
        int charsToNextTurnover = SubMod26( GetNextTurnover( rings, turns ), rings.r ) + 1;
        if( ( messagePosition & 31 ) + charsToNextTurnover < 32 ) {
            // set r ring on positoin (like buttons were pressed nextMTurn times)
            rings.r = GetNextTurnover( rings, turns );
            CalculateMaskAvx2( i, messagePosition & 31, ( messagePosition + charsToNextTurnover ) & 31 );
            // we are still decoding same chunk of message
            messagePosition += charsToNextTurnover;
        }
        else {
            // turn r until messagePosition is next multiple of 32 mod 26
            rAtFirstPos = AddMod26( rAtFirstPos, 31 % 26 );
            rings.r     = rAtFirstPos;
            Step1( &rAtFirstPos );
            CalculateMaskAvx2( i, messagePosition & 31, 32 );
            // next chunk of message, but only R ring was turning
            messagePosition = ( messagePosition + 32 ) & ~31;
            // update turnovers
            PathLookupAvx2.nextBite[biteCounter++] = i + 1;
            if ( biteCounter >= bitesLimit ){
                break;
            }
        }

        StepAllRings( &rings, turns );
    }
}

void prepare_decoder_lookup_M_H3_avx2( const struct Key *const restrict key, int len ) {
    PrepareDecoderLookupAvx2( &CalculatePermutationMap3Rotors, key, len );
}

void prepare_decoder_lookup_ALL_avx2( const struct Key *const restrict key, int len ) {
    PrepareDecoderLookupAvx2( &CalculatePermutationMap4Rotors, key, len );
}
