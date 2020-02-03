#pragma once


#include "global.h"
#include "key.h"


struct RingIterator {
    struct RingsState* const state;
    const struct RingType m, r;
    bool overflow;
};

PURE_FUNCTION
static inline bool
RingIterator_equ( struct RingIterator l, struct RingIterator r ) {
    if( l.overflow || r.overflow ) return l.overflow == r.overflow;
    return RingsState_equ( *l.state, *r.state );
}


static inline void
next_ring( struct RingIterator* i ) {
    i->state->r.delta++;
    if( i->state->r.delta < ( i->r.type > 5 ) ? 13 : 26 ) return;
    i->state->r.delta = 0;

    i->state->m.delta++;
    if( i->state->m.delta < ( i->m.type > 5 ) ? 13 : 26 ) return;
    i->state->m.delta = 0;

    i->overflow = true;
}
