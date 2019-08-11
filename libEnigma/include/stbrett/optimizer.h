#pragma once

#include <stdint.h>

#include "echar/echar.h"
#include "key.h"
#include "score.h"

typedef uint32_t stbrett_optimize_f(
    const struct echar var[26],
    struct Key* ckey,
    int len,
    const enigma_score_function_t* scoring
);
