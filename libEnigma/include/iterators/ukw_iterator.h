#pragma once


#include "global.h"
#include "key.h"


struct UkwIterator {
    struct UkwType ukw;
    bool overflow;
};

CONST_FUNCTION
static inline struct UkwIterator
ukw_overflow() {
    return ( struct UkwIterator ){
        .ukw = {0},
        .overflow = true,
    };
}


static inline bool
UkwIterator_equ( struct UkwIterator l, struct UkwIterator r ) {
    if( l.overflow || r.overflow ) return l.overflow == r.overflow;
    return UkwType_equ( l.ukw, r.ukw );
}


static inline struct UkwIterator
next_ukw( struct UkwIterator i, enum ModelType_t model ) {
    i.ukw.type++;
    if( model == EnigmaModel_H && i.ukw.type > UkwType_C ) {
        i.overflow = true;
        i.ukw.type = UkwType_A;
    } else if( model == EnigmaModel_M3 && i.ukw.type > UkwType_C ) {
        i.overflow = true;
        i.ukw.type = UkwType_B;
    } else if( model == EnigmaModel_M4 && i.ukw.type > UkwType_C_Thin ) {
        i.overflow = true;
        i.ukw.type = UkwType_B_Thin;
    }
    return i;
}
