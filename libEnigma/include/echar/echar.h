#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "charmap.h"
#include "echar_data.h"
#include "echar_delta_data.h"
#include "global.h"
#include "ModMath.h"

static inline
struct echar make_echar( int8_t zero_based ){
    return (struct echar) { .encoded = zero_based };
}

static inline
bool
echar_can_make_from_ascii( unsigned char ascii ){
    if( 'a' <= ascii && ascii <= 'z') return true;
    if( 'A' <= ascii && ascii <= 'Z') return true;
    return false;
}

static inline
struct echar make_echar_ascii( unsigned char ascii ){
    char enc;
    if( 'a' <= ascii && ascii <= 'z') enc = ascii - 'a';
    else if( 'A' <= ascii && ascii <= 'Z') enc = ascii - 'A';
    else enc = 26;
    return (struct echar) { .encoded = enc };
}

CONST_FUNCTION
static inline
char echar_to_alpha( struct echar e ){
    return e.encoded + 'a';
}

static inline
char echar_to_ALPHA( struct echar e ){
    return e.encoded + 'A';
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
v16qu
v16_echar_0_based_index( union v16_echar v ){
    return (v16qu)(v.vector);
}

CONST_FUNCTION
static inline
union v16_echar
v16_echar_add_delta( union v16_echar c, union v16_echar_delta add ){
    v16qs r = AddMod26_v16qs_v16qu( c.vector, add.vector );
    return (union v16_echar){ .vector = r };
}

CONST_FUNCTION
static inline
union v16_echar
v16_echar_sub_delta( union v16_echar c, union v16_echar_delta sub ){
    v16qs r = SubMod26_v16qs_v16qu( c.vector, sub.vector );
    return (union v16_echar){ .vector = r };
}

CONST_FUNCTION
static inline
v32qu
v32_echar_0_based_index( union v32_echar v ){
    return (v32qu)(v.vector);
}

CONST_FUNCTION
static inline
union v32_echar
v32_echar_add_delta( union v32_echar c, union v32_echar_delta add ){
    v32qs r = AddMod26_v32qs_v32qu( c.vector, add.vector );
    return (union v32_echar){ .vector = r };
}

CONST_FUNCTION
static inline
union v32_echar
v32_echar_sub_delta( union v32_echar c, union v32_echar_delta sub ){
    v32qs r = SubMod26_v32qs_v32qu( c.vector, sub.vector );
    return (union v32_echar){ .vector = r };
}
