#pragma once

#include <x86intrin.h>

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
    STATIC_ASSERT( offsetof( struct RingsState, r ) == 0, "r is the last member." );
    union x {
        struct RingsState rs;
        uint32_t bits;
    } in = {.rs = *i.state};

    uint32_t pushed;
    uint32_t result;
    if( model == EnigmaModel_M4 ) {
        const uint32_t pushers = in.bits | 0x60E0E0E0u;
        pushed = pushers + 0x06060607u;
        const uint32_t masked_out_6s = ( pushed >> 4 ) & 0x06060606u; // 0 if overflow else 6
        const uint32_t normal_range_with_pushers = pushed - masked_out_6s;
        result = normal_range_with_pushers & 0x1F1F1F1F;
    } else if( model == EnigmaModel_M3 || model == EnigmaModel_H ) {
        const uint32_t pushers = in.bits | 0x7FE0E0E0u;
        pushed = pushers + 0x060607u;
        const uint32_t masked_out_6s = ( pushed >> 4 ) & 0x060606u; // 0 if overflow else 6
        const uint32_t normal_range_with_pushers = pushed - masked_out_6s;
        result = normal_range_with_pushers & 0x1F1F1F;
    } else {
        UNREACHABLE();
    }
    union x out = {.bits = result};
    *i.state = out.rs;
    i.overflow = (int32_t)pushed < 0;

    return i;
}
