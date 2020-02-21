#pragma once

#include "global.h"
#include "key.h"


struct MesgIterator {
    struct RingsState* state;
    bool overflow;
};


CONST_FUNCTION
static inline struct MesgIterator
mesg_overflow() {
    return ( struct MesgIterator ){
        .state = NULL,
        .overflow = true,
    };
}


PURE_FUNCTION
static inline bool
MesgIterator_equ( struct MesgIterator l, struct MesgIterator r ) {
    if( l.overflow || r.overflow ) return l.overflow == r.overflow;
    return RingsState_equ( *l.state, *r.state );
}


PURE_FUNCTION
static inline struct MesgIterator next_mesg( struct MesgIterator i, const enum ModelType_t model ) {
    i.state->r.delta++;
    if( i.state->r.delta < 26 ) return i;
    i.state->r.delta = 0;

    i.state->m.delta++;
    if( i.state->m.delta < 26 ) return i;
    i.state->m.delta = 0;

    i.state->l.delta++;
    if( i.state->l.delta < 26 ) return i;
    i.state->l.delta = 0;

    if( model != EnigmaModel_M4 ) {
        i.overflow = true;
        return i;
    }

    i.state->g.delta++;
    if( ( i.state->g.delta ) < 26 ) return i;
    i.state->g.delta = 0;
    i.overflow = true;
    return i;
}
