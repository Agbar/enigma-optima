#pragma once

#include <assert.h>
#include <stdbool.h>

#include "echar_delta_data.h"
#include "turnover_data.h"

#include "ModMath.h"

enum comparison_result
{
    cmp_less = -1,
    cmp_equal = 0,
    cmp_greater = 1
};

static inline
bool
echar_delta_can_make_from_ascii( unsigned char ascii ){
    if( 'a' <= ascii && ascii <= 'z') return true;
    if( 'A' <= ascii && ascii <= 'Z') return true;
    return false;
}

struct echar_delta
make_echar_delta_ascii( unsigned char ascii );

static inline
struct echar_delta
make_echar_delta_turnover( struct turnover t ){
    return (struct echar_delta ){ .delta = t.notch };
}

static inline
void echar_delta_rot_1( struct echar_delta* char_delta ){
    IncrementModU( &char_delta->delta, 26 );
}

static inline
void echar_delta_rot_13( struct echar_delta* char_delta ){
    char_delta->delta = AddMod26( char_delta->delta, 13 );
}

static inline
void echar_delta_rot_15( struct echar_delta* char_delta ){
    char_delta->delta = AddMod26( char_delta->delta, 15 );
}

static inline
void echar_delta_rot_31( struct echar_delta* char_delta ){
    char_delta->delta = AddMod26( char_delta->delta, 31 % 26 );
}

static inline
enum comparison_result
echar_delta_cmp( struct echar_delta l, struct echar_delta r ){
    if( l.delta == r.delta ) return cmp_equal;
    if( l.delta <  r.delta ) return cmp_less;
    return cmp_greater;
}

static inline
struct echar_delta
echar_delta_sub( struct echar_delta minuend,  struct echar_delta subtrahend ){
    assert( minuend.delta < 26 );
    assert( subtrahend.delta < 26 );
    int8_t diff = minuend.delta - subtrahend.delta;
    if( diff < 0 ) diff += 26;
    return (struct echar_delta) { .delta = (uint8_t)diff };
}

static inline
struct echar_delta
echar_delta_invert( struct echar_delta d ){
    assert( d.delta < 26 );
    int8_t inv = -d.delta;
    if( inv < 0 ) inv += 26;
    return (struct echar_delta) { .delta = (uint8_t)inv };
}

char echar_delta_to_ALPHA( struct echar_delta d );

static inline
union v16_echar_delta
v16_echar_delta_rot_16( union v16_echar_delta ecd ){
    ecd.vector = AddMod26_v16qu_uint8( ecd.vector, 16 );
    return ecd;
}

static inline
union v32_echar_delta
v32_echar_delta_rot_32( union v32_echar_delta ecd ){
    ecd.vector = AddMod26_v32qu_uint8( ecd.vector , 32 % 26 );
    return ecd;
}
