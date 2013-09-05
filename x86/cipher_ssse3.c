#pragma GCC target ("ssse3")
#include <immintrin.h>
#include <stdbool.h>
#include <string.h>

#include "cipher_ssse3.h"
#include "cipher_ssse3_inlines.h"

#ifndef __SSSE3__
# error SSSE3 not defined
#endif

void prepare_decoder_lookup_M_H3_ssse3( const Key *key, int len );
void prepare_decoder_lookup_ALL_ssse3( const Key *key, int len );
inline extern
v16qi enigma_cipher_decode_ssse3( int biteNumber, int lookupNumber, const Key* const restrict key );
inline extern
v16qi PermuteV16qi(const PermutationMap_t* map, v16qi vec );

enigma_cipher_function_t enigma_cipher_decoder_lookup_ssse3 = {prepare_decoder_lookup_M_H3_ssse3, prepare_decoder_lookup_ALL_ssse3};

struct PathLookupSsse3_t PathLookupSsse3;

typedef void CalculatePermutationMap_f( PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key );

void CalculateLookup( int lookupNumber, struct RingsState rings, uint8_t rOffsetAtFirst, const Key* const restrict key, CalculatePermutationMap_f* calculatePermutationMap )
{
    struct LookupChunk_t* cLkp = &PathLookupSsse3.lookups[lookupNumber];
    int k;

    // calculate offsets used for cyclic permutation
    v16qi rOffsets;
    for( k = 0; k < 16 ; k++ )
    {
        rOffsets[k] = rOffsetAtFirst++;
    }
    rOffsets -= ( rOffsets >= 26 ) & 26;
    cLkp->rRingOffset = rOffsets;

    // calculate m,l,(g),u,(g-1),l-1,m-1 mapping
    calculatePermutationMap( &PathLookupSsse3.lookups[lookupNumber].mapping, rings, key );
}

inline extern
void CalculatePermutationMap3Rotors( PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key ) {
    int k;
    for( k = 0; k < 26; k++ ) {
        int8_t c = k;
        c = wal[key->slot.m][c + rings.m] - rings.m + 26;
        c = wal[key->slot.l][c + rings.l] - rings.l + 26;
        c = ukw[key->ukwnum][c];
        c = rev_wal[key->slot.l][c + rings.l] - rings.l + 26;
        c = rev_wal[key->slot.m][c + rings.m] - rings.m;
        if( c < 0 ) {
            c += 26;
        }
        map->letters[k] = c;
    }
    FixPermutationMapTail( map );
}

inline extern
void CalculatePermutationMap4Rotors( PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key ) {
    int k;
    for( k = 0; k < 26; k++ ) {
        int8_t c = k;
        c = wal[key->slot.m][c + rings.m] - rings.m + 26;
        c = wal[key->slot.l][c + rings.l] - rings.l + 26;
        c = wal[key->slot.g][c + rings.g] - rings.g + 26;
        c = ukw[key->ukwnum][c];
        c = rev_wal[key->slot.g][c + rings.g] - rings.g + 26;
        c = rev_wal[key->slot.l][c + rings.l] - rings.l + 26;
        c = rev_wal[key->slot.m][c + rings.m] - rings.m;
        if( c < 0 ) {
            c += 26;
        }
        map->letters[k] = c;
    }
    FixPermutationMapTail( map );
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

inline extern void StepAllRings( struct RingsState* restrict rings, struct Turnovers_t turns )
{
    // check if m,l rings will be turned
    bool p2 = 0, p3 = 0;
    if( rings->r == turns.r || rings->r == turns.r2 ) {
        p2 = 1;
    }
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if( rings->m == turns.m || rings->m == turns.m2 ) {
        p3 = 1;
        p2 = 1;
    }
    // turn rings
    Step1( &rings->r );
    if( p2 ) {
        Step1( &rings->m );
    }
    if( p3 ) {
        Step1( &rings->l );
    }
}

inline extern void CopyRRing2Lookup( const Key* const restrict key )
{
    // setup r_rings forward and backward.
    memcpy( PathLookupSsse3.r_ring[0].letters, wal[key->slot.r], 32 );
    memcpy( PathLookupSsse3.r_ring[1].letters, rev_wal[key->slot.r], 32 );
}


int8_t GetNextTurnover( const struct RingsState rings, const struct Turnovers_t turns )
{
    // turnover on M-ring
    if ( turns.m2 == rings.m || turns.m == rings.m ) return rings.r; //Turnover now!
    // normal
    return ( turns.r2 != -1 && rings.r > turns.r ) ? turns.r2 : turns.r; // Turnover caused by R-ring
}

#define SECOND_TURNOVER_POINT 25

inline extern
void PrepareDecoderLookup( CalculatePermutationMap_f* calculateMap, const Key *const restrict key, int len ) {
    CopyRRing2Lookup( key );

    // rings.r will be a position of R-ring for current lookup chunk
    struct RingsState rings = {
        .r = SubMod26( key->mesg.r, key->ring.r ),
        .m = SubMod26( key->mesg.m, key->ring.m ),
        .l = SubMod26( key->mesg.l, key->ring.l ),
        .g = SubMod26( key->mesg.g, key->ring.g ),
    };

    /* calculate turnover points from ring settings */
    struct Turnovers_t turns = {
        .r = SubMod26( wal_turn[key->slot.r], key->ring.r ),
        .m = SubMod26( wal_turn[key->slot.m], key->ring.m ),
    };

    /* second turnover points for wheels 6,7,8 */
    if( key->slot.r > 5 ) {
        turns.r2 = SubMod26( SECOND_TURNOVER_POINT, key->ring.r );
    }
    else {
        turns.r2 = -1;
    }
    if( key->slot.m > 5 ) {
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

    int i;
    int bitesLimit = ( len + 15 )/ 16;
    for( i = 0; i < 24; i++ ) //fixme
    {
        CalculateLookup( i, rings, rAtFirstPos, key, calculateMap );

        // check wether next M-ring turn is within current text bite
        int charsToNextTurnover = SubMod26( GetNextTurnover( rings, turns ), rings.r ) + 1;
        if( ( messagePosition & 15 ) + charsToNextTurnover < 16 ) {
            // set r ring on positoin (like buttons were pressed nextMTurn times)
            rings.r = GetNextTurnover( rings, turns );
            CalculateMask( i, messagePosition & 15, ( messagePosition + charsToNextTurnover ) & 15 );
            // we are still decoding same chunk of message
            messagePosition += charsToNextTurnover;
        }
        else {
            // turn r until messagePosition is next multiple of 16
            rAtFirstPos = AddMod26( rAtFirstPos, 15 );
            rings.r     = rAtFirstPos;
            Step1( &rAtFirstPos );
            CalculateMask( i, messagePosition & 15, 16 );
            // next chunk of message, but only R ring was turning
            messagePosition = ( messagePosition + 16 ) & ~15;
            // update turnovers
            PathLookupSsse3.nextBite[biteCounter++] = i;
            if ( biteCounter >= bitesLimit ){
                break;
            }
        }

        StepAllRings( &rings, turns );
    }
}

void prepare_decoder_lookup_M_H3_ssse3( const Key *const restrict key, int len ) {
    PrepareDecoderLookup( &CalculatePermutationMap3Rotors, key, len );
}

void prepare_decoder_lookup_ALL_ssse3( const Key *key, int len ) {
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
