#pragma once


#include "global.h"
#include "key.h"


struct UkwIterator {
    struct UkwType ukw;
    bool overflow;
    enum ModelType_t model;
};

CONST_FUNCTION
static inline struct UkwIterator
ukw_overflow() {
    return ( struct UkwIterator ){
        .ukw = {UkwType_A}, // any
        .overflow = true,
        .model = EnigmaModel_H,
    };
}


static inline bool
UkwIterator_equ( struct UkwIterator l, struct UkwIterator r ) {
    if( l.overflow || r.overflow ) return l.overflow == r.overflow;
    return UkwType_equ( l.ukw, r.ukw );
}


CONST_FUNCTION
static inline struct UkwIterator
next_ukw( struct UkwIterator i ) {
    // C is more permisive than C++ regarding enums
    i.ukw.type = ( enum ukw_type_enum )( (int)i.ukw.type + 1 );
    if( i.model == EnigmaModel_H && i.ukw.type > UkwType_C ) {
        i.overflow = true;
        i.ukw.type = UkwType_A;
    } else if( i.model == EnigmaModel_M3 && i.ukw.type > UkwType_C ) {
        i.overflow = true;
        i.ukw.type = UkwType_B;
    } else if( i.model == EnigmaModel_M4 && i.ukw.type > UkwType_C_Thin ) {
        i.overflow = true;
        i.ukw.type = UkwType_B_Thin;
    }
    return i;
}
