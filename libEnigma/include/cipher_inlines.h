#ifndef CIPHER_INLINES_HEADER_INCLUDED
#define CIPHER_INLINES_HEADER_INCLUDED

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "key.h"

#include "echar/echar_delta.h"
#include "echar/turnover.h"

static inline
void StepAllRings( struct RingsState* const restrict rings, const struct Turnovers_t turns )
{
    // check if m,l rings will be turned
    bool p2 = 0, p3 = 0;
    if( turnover_eq_echar_delta( turns.r, rings->r ) || turnover_eq_echar_delta( turns.r2, rings->r ) ) {
        p2 = 1;
    }
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if( turnover_eq_echar_delta( turns.m, rings->m ) || turnover_eq_echar_delta( turns.m2, rings->m ) ) {
        p3 = 1;
        p2 = 1;
    }
    // turn rings
    echar_delta_rot_1( &rings->r );
    if( p2 ) {
        echar_delta_rot_1( &rings->m );
    }
    if( p3 ) {
        echar_delta_rot_1( &rings->l );
    }
}

static inline
void CopyRRing2Lookup( const struct Key* const restrict key, union PermutationMap_t rRings[2] )
{
    // setup r_rings forward and backward.
    memcpy( rRings[0].letters, wal[key->slot.r.type].flat, 32 );
    memcpy( rRings[1].letters, rev_wal[key->slot.r.type].flat, 32 );
}

//! \brief Return position of R ring on next turnover
static inline
struct echar_delta
GetNextTurnover( const struct RingsState rings, const struct Turnovers_t turns )
{
    // turnover caused by M-ring (double step)
    if ( turnover_eq_echar_delta( turns.m2,rings.m ) || turnover_eq_echar_delta( turns.m, rings.m ) ) return rings.r; //Turnover now!
    // normal Turnover caused by R-ring
    if( turnover_eq_absent( turns.r2 ) )
    {
        return make_echar_delta_turnover( turns.r );
    }
    // double notched R-ring
    assert( turns.r2.notch > turns.r.notch );
    struct turnover next = turnover_select_next( rings.r, turns.r, turns.r2);
    return make_echar_delta_turnover( next );
}

static inline
void CalculatePermutationMap3Rotors( union PermutationMap_t* const restrict map, struct RingsState rings, const struct Key* const restrict key ) {
    struct echar_delta
        m_offset = rings.m ,
        l_offset = rings.l ;
    struct echar_delta 
        m_l_offset = echar_delta_sub( l_offset, m_offset ),
        inv_l_offset = echar_delta_invert( l_offset ),
        l_m_offset = echar_delta_sub( m_offset, l_offset );

    for( int k = 0; k < 26; k++ ) {
        struct echar c = make_echar( k );
        c = wal[key->slot.m.type].flat[ double_index( c,     m_offset ) ];
        c = wal[key->slot.l.type].flat[ double_index( c,   m_l_offset ) ];
        c = ukw[key->ukwnum].flat[ double_index( c, inv_l_offset ) ];
        c = rev_wal[key->slot.l.type].flat[ double_index( c,   l_offset ) ];
        c = rev_wal[key->slot.m.type].flat[ double_index( c, l_m_offset ) ];
        c = echar_sub_delta( c, m_offset );
        map->letters[k] = c;
    }
    FixPermutationMapTail( map );
}

static inline
void CalculatePermutationMap4Rotors( union PermutationMap_t* const restrict map, struct RingsState rings, const struct Key* const restrict key ) {
    struct echar_delta
        m_offset = rings.m ,
        l_offset = rings.l ,
        g_offset = rings.g ;
    struct echar_delta 
        m_l_offset = echar_delta_sub( l_offset, m_offset ),
        l_g_offset = echar_delta_sub( g_offset, l_offset ),
        inv_g_offset = echar_delta_invert( g_offset ),
        g_l_offset = echar_delta_sub( l_offset, g_offset ),
        l_m_offset = echar_delta_sub( m_offset, l_offset );

    for( int k = 0; k < 26; k++ ) {
        struct echar c = make_echar( k );
        c = wal[key->slot.m.type].flat[ double_index( c,     m_offset ) ];
        c = wal[key->slot.l.type].flat[ double_index( c,   m_l_offset ) ];
        c = wal[key->slot.g.type].flat[ double_index( c,   l_g_offset ) ];
        c = ukw[key->ukwnum].flat[ double_index( c, inv_g_offset ) ];
        c = rev_wal[key->slot.g.type].flat[ double_index( c,   g_offset ) ];
        c = rev_wal[key->slot.l.type].flat[ double_index( c, g_l_offset ) ];
        c = rev_wal[key->slot.m.type].flat[ double_index( c, l_m_offset ) ];
        c = echar_sub_delta( c, m_offset );
        map->letters[k] = c;
    }
    FixPermutationMapTail( map );
}

#endif
