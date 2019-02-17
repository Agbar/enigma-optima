#ifndef CIPHER_INLINES_HEADER_INCLUDED
#define CIPHER_INLINES_HEADER_INCLUDED

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "key.h"

static inline
void StepAllRings( struct RingsState* const restrict rings, const struct Turnovers_t turns )
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

static inline
void CopyRRing2Lookup( const Key* const restrict key, union PermutationMap_t rRings[2] )
{
    // setup r_rings forward and backward.
    memcpy( rRings[0].letters, wal[key->slot.r].flat, 32 );
    memcpy( rRings[1].letters, rev_wal[key->slot.r].flat, 32 );
}

//! \brief Return position of R ring on next turnover
static inline
int8_t GetNextTurnover( const struct RingsState rings, const struct Turnovers_t turns )
{
    // turnover caused by M-ring (double step)
    if ( turns.m2 == rings.m || turns.m == rings.m ) return rings.r; //Turnover now!
    // normal Turnover caused by R-ring
    if( turns.r2 == -1)
    {
        return turns.r;
    }
    // double notched R-ring
    assert( turns.r2 > turns.r );
    if( rings.r <= turns.r )
    {
        return turns.r;
    }
    else if ( rings.r <= turns.r2 )
    {
        return turns.r2;
    }
    else
    {
        return turns.r;
    }
}

static inline
void CalculatePermutationMap3Rotors( union PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key ) {
    struct echar_delta
        m_offset = { .delta = (uint8_t)rings.m },
        l_offset = { .delta = (uint8_t)rings.l };
    struct echar_delta 
        m_l_offset = char_delta_sub( l_offset, m_offset ),
        inv_l_offset = char_delta_invert( l_offset ),
        l_m_offset = char_delta_sub( m_offset, l_offset );

    for( int k = 0; k < 26; k++ ) {
        struct echar c = { .encoded = (int8_t)k };
        c = wal[key->slot.m].flat[ double_index( c,     m_offset ) ];
        c = wal[key->slot.l].flat[ double_index( c,   m_l_offset ) ];
        c = ukw[key->ukwnum].flat[ double_index( c, inv_l_offset ) ];
        c = rev_wal[key->slot.l].flat[ double_index( c,   l_offset ) ];
        c = rev_wal[key->slot.m].flat[ double_index( c, l_m_offset ) ];
        c = echar_sub_delta( c, m_offset );
        map->letters[k] = c.encoded;
    }
    FixPermutationMapTail( map );
}

static inline
void CalculatePermutationMap4Rotors( union PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key ) {
    struct echar_delta
        m_offset = { .delta = (uint8_t)rings.m },
        l_offset = { .delta = (uint8_t)rings.l },
        g_offset = { .delta = (uint8_t)rings.g };
    struct echar_delta 
        m_l_offset = char_delta_sub( l_offset, m_offset ),
        l_g_offset = char_delta_sub( g_offset, l_offset ),
        inv_g_offset = char_delta_invert( g_offset ),
        g_l_offset = char_delta_sub( l_offset, g_offset ),
        l_m_offset = char_delta_sub( m_offset, l_offset );

    for( int k = 0; k < 26; k++ ) {
        struct echar c = { .encoded = (int8_t)k };
        c = wal[key->slot.m].flat[ double_index( c,     m_offset ) ];
        c = wal[key->slot.l].flat[ double_index( c,   m_l_offset ) ];
        c = wal[key->slot.g].flat[ double_index( c,   l_g_offset ) ];
        c = ukw[key->ukwnum].flat[ double_index( c, inv_g_offset ) ];
        c = rev_wal[key->slot.g].flat[ double_index( c,   g_offset ) ];
        c = rev_wal[key->slot.l].flat[ double_index( c, g_l_offset ) ];
        c = rev_wal[key->slot.m].flat[ double_index( c, l_m_offset ) ];
        c = echar_sub_delta( c, m_offset );
        map->letters[k] = c.encoded;
    }
    FixPermutationMapTail( map );
}

#endif
