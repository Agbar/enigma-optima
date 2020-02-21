#pragma once


#include "key.h"

#define SlotIterator_bits_overflow 0x80

struct SlotIterator {
    struct {
        uint8_t og, l, m, r;
    } bits;
    struct RingTypes* const state;
    void ( *const next )( struct SlotIterator* const );
};


const struct SlotIterator slot_end = {.bits = {.og = SlotIterator_bits_overflow}};


PURE_FUNCTION
static inline bool
SlotIterator_equ( struct SlotIterator l, struct SlotIterator r ) {
    const uint8_t l_ov = l.bits.og & SlotIterator_bits_overflow;
    const uint8_t r_ov = r.bits.og & SlotIterator_bits_overflow;
    if( l_ov || r_ov ) return l_ov == r_ov;
    return RingTypes_equ( *l.state, *r.state );
}


static inline struct RingType bits_to_RingType( uint8_t bits ) {
    int trailingBits = __builtin_ctz( bits );
    if( trailingBits > 8 ) UNREACHABLE();
    const struct RingType ringTypes[ 8 ] =
        {{RingType_1}, {RingType_2}, {RingType_3}, {RingType_4}, {RingType_5}, {RingType_6}, {RingType_7}, {RingType_8}};
    return ringTypes[ trailingBits ];
}


static inline uint8_t RingType_to_bits( struct RingType ring ) {
    return 1 << ( ring.type - 1 );
}


static inline uint8_t
bits_adjust_next_r( const struct SlotIterator* const i, uint8_t candidate, const uint8_t mask ) {
    uint8_t non_r = i->bits.l | i->bits.m;
    uint8_t r_skip_others = non_r + candidate;
    return r_skip_others & ~non_r & mask;
}


static inline uint8_t
bits_adjust_next_m( const struct SlotIterator* const i, uint8_t candidate, const uint8_t mask ) {
    uint8_t m_skip_l = i->bits.l + candidate;
    return m_skip_l & ~i->bits.l & mask;
}

static inline uint8_t
bits_adjust_next_l( uint8_t candidate, const uint8_t mask ) {
    return candidate & mask;
}


static inline bool
next_slot_common( struct SlotIterator* const i, const uint8_t mask ) {
    i->bits.r = bits_adjust_next_r( i, i->bits.r << 1, mask );
    if( i->bits.r ) {
        i->state->r = bits_to_RingType( i->bits.r );
        return false;
    }

    i->bits.m = bits_adjust_next_m( i, i->bits.m << 1, mask );
    if( i->bits.m ) {
        i->bits.r = bits_adjust_next_r( i, 1, mask );
        i->state->m = bits_to_RingType( i->bits.m );
        i->state->r = bits_to_RingType( i->bits.r );
        return false;
    };

    i->bits.l = bits_adjust_next_l( i->bits.l << 1, mask );
    if( i->bits.l ) {
        i->bits.m = bits_adjust_next_m( i, 1, mask );
        i->bits.r = bits_adjust_next_r( i, 1, mask );
        i->state->m = bits_to_RingType( i->bits.m );
        i->state->r = bits_to_RingType( i->bits.r );
        i->state->l = bits_to_RingType( i->bits.l );
        return false;
    }
    i->bits.l = 1;
    i->bits.m = 2;
    i->bits.r = 4;
    i->state->l = bits_to_RingType( i->bits.l );
    i->state->m = bits_to_RingType( i->bits.m );
    i->state->r = bits_to_RingType( i->bits.r );
    return true;
}

static inline void
next_slot_h( struct SlotIterator* const i ) {
    if( next_slot_common( i, 0b00011111 ) ) {
        i->bits.og |= SlotIterator_bits_overflow;
    }
}


static inline void
next_slot_m3( struct SlotIterator* const i ) {
    if( next_slot_common( i, 0xFF ) ) {
        i->bits.og |= SlotIterator_bits_overflow;
    }
}


static inline void
next_slot_m4( struct SlotIterator* const i ) {
    if( next_slot_common( i, 0xFF ) ) {
        i->bits.og ^= 3;
        i->state->g.type = ( i->bits.og & 3 ) == 1 ? GreekRingType_Beta : GreekRingType_Gamma;
        if( ( i->bits.og & 3 ) == 1 ) i->bits.og |= SlotIterator_bits_overflow;
    }
}

static inline struct SlotIterator
init_SlotIterator( struct Key* k ) {
    void ( *next )( struct SlotIterator* const );
    uint8_t mask;
    switch( k->model ) {
    case EnigmaModel_H:
        next = next_slot_h;
        mask = 0b11111;
        break;
    case EnigmaModel_M3:
        next = next_slot_m3;
        mask = 0xFF;
        break;
    case EnigmaModel_M4:
        next = next_slot_m4;
        mask = 0xFF;
        break;
    default:
        UNREACHABLE();
    }
    struct SlotIterator ret = {.state = &k->slot,
                               .next = next};

    struct RingTypes slot = k->slot;
    if( k->model == EnigmaModel_M4 ) {
        ret.bits.og = ( slot.g.type == GreekRingType_Beta ) ? 1 : 2;
    }
    ret.bits.l = RingType_to_bits( slot.l );
    ret.bits.m = bits_adjust_next_m( &ret, RingType_to_bits( slot.m ), mask );
    ret.bits.r = bits_adjust_next_r( &ret, RingType_to_bits( slot.r ), mask );
    ret.state->l = bits_to_RingType( ret.bits.l );
    ret.state->m = bits_to_RingType( ret.bits.m );
    ret.state->r = bits_to_RingType( ret.bits.r );
    return ret;
}
