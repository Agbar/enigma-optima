#pragma GCC target ("ssse3")


#include <immintrin.h>
#include <stdbool.h>
#include <string.h>

#include "x86/cipherSsse3.h"
#include "cipher_inlines.h"

void prepare_decoder_lookup_M_H3_ssse3( const struct Key *key, int len );
void prepare_decoder_lookup_ALL_ssse3( const struct Key *key, int len );

enigma_cipher_function_t enigma_cipher_decoder_lookup_ssse3 = {prepare_decoder_lookup_M_H3_ssse3, prepare_decoder_lookup_ALL_ssse3};

struct PathLookupSsse3_t PathLookupSsse3;

typedef void CalculatePermutationMap_f( union PermutationMap_t* const restrict map, struct RingsState rings, const struct Key* restrict key );

void CalculateLookup( int lookupNumber, struct RingsState rings, const struct Key* restrict key, CalculatePermutationMap_f* calculatePermutationMap );
void CalculateMask( size_t lookupNumber, int8_t begin, int8_t end );
void CalculateRRingOffets( struct echar_delta rOffsetAtFirst );
void PrintLookup( FILE* file, v32qi vec, const char* const restrict name );

void CalculateRRingOffets( struct echar_delta rOffsetAtFirst ) {
    int k;
    uint_fast8_t r = rOffsetAtFirst.delta;
    // calculate offsets used for cyclic permutation
    v16qu rOffsets;
    for( k = 0; k < 16 ; k++ )
    {
        rOffsets[k] = r++;
    }
    rOffsets -= ( rOffsets >= 26 ) & 26;
    PathLookupSsse3.firstRRingOffset.vector = rOffsets;
}

void CalculateLookup( int lookupNumber, struct RingsState rings, const struct Key* const key, CalculatePermutationMap_f* calculatePermutationMap )
{
    // calculate m,l,(g),u,(g-1),l-1,m-1 mapping
    calculatePermutationMap( &PathLookupSsse3.lookups[lookupNumber].mapping, rings, key );
}


void CalculateMask( size_t lookupNumber, int8_t begin, int8_t end )
{
    // erase previous contents
    PathLookupSsse3.lookups[lookupNumber].mask ^= PathLookupSsse3.lookups[lookupNumber].mask;
    int8_t current = begin;
    for( ; current < end; current++ ) {
        PathLookupSsse3.lookups[lookupNumber].mask[current] = ~0;
    }
}

#define SECOND_TURNOVER_POINT 25

inline extern
void PrepareDecoderLookup( CalculatePermutationMap_f* calculateMap, const struct Key *const restrict key, int len ) {
    CopyRRing2Lookup( key, PathLookupSsse3.r_ring );

    // rings.r will be a position of R-ring for current lookup chunk
    struct RingsState rings = {
        .r = echar_delta_sub( key->mesg.r, key->ring.r ),
        .m = echar_delta_sub( key->mesg.m, key->ring.m ),
        .l = key->mesg.l,
        .g = key->mesg.g,
    };

    /* calculate turnover points from ring settings */
    struct Turnovers_t turns = {
        .r = turnover_sub_echar_delta( wal_turn[key->slot.r.type], key->ring.r ),
        .m = turnover_sub_echar_delta( wal_turn[key->slot.m.type], key->ring.m ),
    };

    /* second turnover points for wheels 6,7,8 */
    if( key->slot.r.type > 5 ) {
        turns.r2 = turnover_sub_echar_delta( turnover_second_notch(), key->ring.r );
    }
    else {
        turns.r2 = turnover_absent();
    }
    if( key->slot.m.type > 5 ) {
        turns.m2 = turnover_sub_echar_delta( turnover_second_notch(), key->ring.m );
    }
    else {
        turns.m2 = turnover_absent();
    }

    // simulate first key-press and get rid of possible first-press l,m-rings turn
    StepAllRings( &rings, turns );
    struct echar_delta rAtFirstPos = rings.r; ///< Right ring state _after_ pressing a button.
    int messagePosition = 0;            ///< Index of first character decoded by current lookup.
    int biteCounter  = 0;               ///< Number of bite currently processed.

    CalculateRRingOffets( rAtFirstPos );

    int i;
    int bitesLimit = ( len + 15 )/ 16;
    for( i = 0; i < 24; i++ ) //fixme
    {
        CalculateLookup( i, rings, key, calculateMap );

        // check whether next M-ring turn is within current text bite
        struct echar_delta delta_next = echar_delta_sub( GetNextTurnover( rings, turns ), rings.r );
        int charsToNextTurnover = delta_next.delta + 1;
        if( ( messagePosition & 15 ) + charsToNextTurnover < 16 ) {
            // set r ring on positoin (like buttons were pressed nextMTurn times)
            rings.r = GetNextTurnover( rings, turns );
            CalculateMask( i, messagePosition & 15, ( messagePosition + charsToNextTurnover ) & 15 );
            // we are still decoding same chunk of message
            messagePosition += charsToNextTurnover;
        }
        else {
            // turn r until messagePosition is next multiple of 16
            echar_delta_rot_15( &rAtFirstPos );
            rings.r = rAtFirstPos;
            echar_delta_rot_1( &rAtFirstPos );
            CalculateMask( i, messagePosition & 15, 16 );
            // next chunk of message, but only R ring was turning
            messagePosition = ( messagePosition + 16 ) & ~15;
            // update turnovers
            PathLookupSsse3.nextBite[biteCounter++] = i + 1;
            if ( biteCounter >= bitesLimit ){
                break;
            }
        }

        StepAllRings( &rings, turns );
    }
}

void prepare_decoder_lookup_M_H3_ssse3( const struct Key *const restrict key, int len ) {
    PrepareDecoderLookup( &CalculatePermutationMap3Rotors, key, len );
}

void prepare_decoder_lookup_ALL_ssse3( const struct Key *const restrict key, int len ) {
    PrepareDecoderLookup( &CalculatePermutationMap4Rotors, key, len );
}

void PrintLookup( FILE* file, v32qi vec, const char* const restrict name )
{
    fprintf( file, "%10s: ", name );
    int i = 0;
    for( ; i < 25; i++ ) {
        fprintf( file, "%2i ", vec[i] );
    }
    fprintf( file, "%2i\n", vec[25] );
    fflush( file );
}
