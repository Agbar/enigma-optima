#pragma once

#include <stdint.h>

#include "config/types.h"
#include "global.h"

struct echar 
{
    int8_t encoded;
};

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

STATIC_ASSERT( sizeof(struct echar) == 1, "echar must be compact" );
STATIC_ASSERT( sizeof(union v16_echar) == 16, "vector and array of characters must have the same length");
STATIC_ASSERT( sizeof(union v32_echar) == 32, "vector and array of characters must have the same length");
