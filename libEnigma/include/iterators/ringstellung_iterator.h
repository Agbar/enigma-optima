#pragma once


#include "global.h"
#include "key.h"


struct RingstellungIterator {
    struct Ringstellung* const state;
    const struct RingType m, r;
    bool overflow;
};


CONST_FUNCTION
static inline struct RingstellungIterator
ring_overflow() {
    return ( struct RingstellungIterator ){
        .state = NULL,
        .m = {RingType_None},
        .r = {RingType_None},
        .overflow = true,
    };
}


PURE_FUNCTION
static inline bool
RingstellungIterator_equ( struct RingstellungIterator l, struct RingstellungIterator r ) {
    if( l.overflow || r.overflow ) return l.overflow == r.overflow;
    return Ringstellung_equ( *l.state, *r.state );
}


static inline void
next_ringstellung( struct RingstellungIterator* i ) {
    i->state->r.delta++;
    const uint8_t r_limit = ( i->r.type > 5 ) ? 13 : 26;
    if( i->state->r.delta < r_limit ) return;
    i->state->r.delta = 0;

    i->state->m.delta++;
    const uint8_t m_limit = ( i->m.type > 5 ) ? 13 : 26;
    if( i->state->m.delta < m_limit ) return;
    i->state->m.delta = 0;

    i->overflow = true;
}
