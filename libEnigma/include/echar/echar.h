#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "echar_data.h"
#include "echar_delta_data.h"

static inline 
struct echar make_echar( int8_t zero_based ){
    return (struct echar) { .encoded = zero_based };
}

static inline
uint_fast8_t echar_0_based_index( struct echar c ){
    return c.encoded;
}

static inline
bool echar_eq( struct echar l, struct echar r ){
    return l.encoded == r.encoded;
}

static inline
bool echar_neq( struct echar l, struct echar r ){
    return l.encoded != r.encoded;
}

static inline
struct echar
echar_sub_delta( struct echar c, struct echar_delta sub ){
    assert( sub.delta < 26 );
    int8_t r = c.encoded - sub.delta;
    if( r < 0 ) r +=26;
    return (struct echar) { .encoded = r };
}