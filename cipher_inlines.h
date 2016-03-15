#ifndef CIPHER_INLINES_HEADER_INCLUDED
#define CIPHER_INLINES_HEADER_INCLUDED

#include <stdbool.h>
#include "common.h"
#include "key.h"

inline
void StepAllRings( struct RingsState* restrict rings, struct Turnovers_t turns )
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

inline
void CopyRRing2Lookup( const Key* const restrict key, PermutationMap_t rRings[2] )
{
    // setup r_rings forward and backward.
    memcpy( rRings[0].letters, wal[key->slot.r], 32 );
    memcpy( rRings[1].letters, rev_wal[key->slot.r], 32 );
}

inline
int8_t GetNextTurnover( const struct RingsState rings, const struct Turnovers_t turns )
{
    // turnover on M-ring
    if ( turns.m2 == rings.m || turns.m == rings.m ) return rings.r; //Turnover now!
    // normal
    return ( turns.r2 != -1 && rings.r > turns.r ) ? turns.r2 : turns.r; // Turnover caused by R-ring
}


inline
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

inline
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

#endif
