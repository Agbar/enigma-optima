#pragma once

#include <stdint.h>

#include "config/types.h"
#include "global.h"

struct echar_delta
{
    uint8_t delta;
};

union v16_echar_delta
{
    struct echar_delta v_ecd[16];
    v16qi vector;
};

union v32_echar_delta
{
    struct echar_delta v_ecd[32];
    v32qi vector;
};

STATIC_ASSERT( sizeof(struct echar_delta) == 1, "enigma_character_delta must be compact" );
STATIC_ASSERT( sizeof(union v16_echar_delta) == 16, "vector and array of character deltas must have the same length");
STATIC_ASSERT( sizeof(union v32_echar_delta) == 32, "vector and array of character deltas must have the same length");
