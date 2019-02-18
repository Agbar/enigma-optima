#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "global.h"
#include "config/types.h"
#include "ModMath.h"

struct echar 
{
    // A - Z letters encoded as numbers in range [-10; 15]
    // This encoding is x86-shuffle-friendly
    int8_t encoded;
};

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

union v16_echar
{
    struct echar v_ec[16];
    v16qi vector;
};

union v32_echar
{
    struct echar v_ec[32];
    v32qi vector;
};

struct echar_delta
{
    uint8_t delta;
};

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
void char_delta_rot_1( struct echar_delta* char_delta ){
    IncrementModU( &char_delta->delta, 26 );
}

static inline
struct echar_delta
char_delta_sub( struct echar_delta minuend,  struct echar_delta subtrahend ){
    assert( minuend.delta < 26 );
    assert( subtrahend.delta < 26 );
    int8_t diff = minuend.delta - subtrahend.delta;
    if( diff < 0 ) diff += 26;
    return (struct echar_delta) { .delta = (uint8_t)diff };
}

static inline
struct echar_delta
char_delta_invert( struct echar_delta d ){
    assert( d.delta < 26 );
    int8_t inv = -d.delta;
    if( inv < 0 ) inv += 26;
    return (struct echar_delta) { .delta = (uint8_t)inv };
}

static inline
struct echar
echar_sub_delta( struct echar c, struct echar_delta sub ){
    assert( sub.delta < 26 );
    int8_t r = c.encoded - sub.delta;
    if( r < 0 ) r +=26;
    return (struct echar) { .encoded = r };
}

union v16_echar_delta
{
    struct echar_delta v_ecd[16];
    v16qi vector;
};

static inline
union v16_echar_delta 
v16_echar_delta_rot_16( union v16_echar_delta ecd ){
    ecd.vector = AddMod26_v16qi_int8( ecd.vector, 16 );
    return ecd;
}

union v32_echar_delta
{
    struct echar_delta v_ecd[32];
    v32qi vector;
};

static inline
union v32_echar_delta
v32_echar_delta_rot_32( union v32_echar_delta ecd ){
    ecd.vector = AddMod26_v32qi_int8( ecd.vector , 32 % 26 );
    return ecd;
}

STATIC_ASSERT( sizeof(struct echar) == 1, "echar must be compact" );
STATIC_ASSERT( sizeof(union v16_echar) == 16, "vector and array of characters must have the same length");
STATIC_ASSERT( sizeof(union v32_echar) == 32, "vector and array of characters must have the same length");
STATIC_ASSERT( sizeof(struct echar_delta) == 1, "enigma_character_delta must be compact" );
STATIC_ASSERT( sizeof(union v16_echar_delta) == 16, "vector and array of character deltas must have the same length");
STATIC_ASSERT( sizeof(union v32_echar_delta) == 32, "vector and array of character deltas must have the same length");
