#pragma once

#include <stdint.h>
#include "global.h"
#include "config/types.h"

struct enigma_character 
{
    // A - Z letters encoded as numbers in range [-10; 15]
    // This encoding is x86-shuffle-friendly
    int8_t encoded;
};

static inline
size_t ec_0_based_index( struct enigma_character c ){
    return c.encoded;
}

union v16_echar
{
    struct enigma_character v_ec[16];
    v16qi vector;
};

union v32_echar
{
    struct enigma_character v_ec[32];
    v32qi vector;
};

struct enigma_char_delta
{
    int8_t delta;
};

union v16_echar_delta
{
    struct enigma_char_delta v_ecd[16];
    v16qi vector;
};

union v32_echar_delta
{
    struct enigma_char_delta v_ecd[32];
    v32qi vector;
};

STATIC_ASSERT( sizeof(struct enigma_character) == 1, "enigma_character must be compact" );
STATIC_ASSERT( sizeof(union v16_echar) == 16, "vector and array of characters must have the same length");
STATIC_ASSERT( sizeof(union v32_echar) == 32, "vector and array of characters must have the same length");
STATIC_ASSERT( sizeof(struct enigma_char_delta) == 1, "enigma_character_delta must be compact" );
STATIC_ASSERT( sizeof(union v16_echar_delta) == 16, "vector and array of character deltas must have the same length");
STATIC_ASSERT( sizeof(union v32_echar_delta) == 32, "vector and array of character deltas must have the same length");
