#include "iterators/slot_iterator.h"


struct SlotIterator
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
