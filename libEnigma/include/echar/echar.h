#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "echar_data.h"
#include "echar_delta_data.h"
#include "global.h"
#include "ModMath.h"

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

CONST_FUNCTION
static inline
struct echar
echar_sub_delta( struct echar c, struct echar_delta sub ){
    assert( sub.delta < 26 );
    int8_t r = c.encoded - sub.delta;
    if( r < 0 ) r +=26;
    return (struct echar) { .encoded = r };
}

CONST_FUNCTION
static inline
union v16_echar
v16_echar_add_delta( union v16_echar c, union v16_echar_delta add ){
    v16qi r = AddMod26_v16qi( c.vector, add.vector );
    return (union v16_echar){ .vector = r };
}

CONST_FUNCTION
static inline
union v16_echar
v16_echar_sub_delta( union v16_echar c, union v16_echar_delta sub ){
    v16qi r = SubMod26_v16qi( c.vector, sub.vector );
    return (union v16_echar){ .vector = r };
}

CONST_FUNCTION
static inline
union v32_echar
v32_echar_add_delta( union v32_echar c, union v32_echar_delta add ){
    v32qi r = AddMod26_v32qi( c.vector, add.vector );
    return (union v32_echar){ .vector = r };
}

CONST_FUNCTION
static inline
union v32_echar
v32_echar_sub_delta( union v32_echar c, union v32_echar_delta sub ){
    v32qi r = SubMod26_v32qi( c.vector, sub.vector );
    return (union v32_echar){ .vector = r };
}
