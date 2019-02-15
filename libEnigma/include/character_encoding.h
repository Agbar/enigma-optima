#pragma once

#include <stdint.h>
#include "config/types.h"

struct enigma_character 
{
    // A - Z letters encoded as numbers in range [-10; 15]
    // This encoding is x86-shuffle-friendly
    int8_t encoded;
};

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
