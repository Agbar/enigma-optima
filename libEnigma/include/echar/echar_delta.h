#pragma once

#include <assert.h>

#include "echar_delta_data.h"
#include "ModMath.h"

static inline
struct echar_delta
make_char_delta_plus_minus( uint8_t plus_offset,  uint8_t minus_offset ){
    assert( plus_offset < 26 );
    assert( minus_offset < 26 );
    int8_t d = plus_offset - minus_offset;
    if( d < 0 ) d += 26;
    return (struct echar_delta) { .delta = (uint8_t)d };
}

static inline
void echar_delta_rot_1( struct echar_delta* char_delta ){
    IncrementModU( &char_delta->delta, 26 );
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

static inline
union v16_echar_delta 
v16_echar_delta_rot_16( union v16_echar_delta ecd ){
    ecd.vector = AddMod26_v16qi_int8( ecd.vector, 16 );
    return ecd;
}

static inline
union v32_echar_delta
v32_echar_delta_rot_32( union v32_echar_delta ecd ){
    ecd.vector = AddMod26_v32qi_int8( ecd.vector , 32 % 26 );
    return ecd;
}
