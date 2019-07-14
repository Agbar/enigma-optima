#pragma once

#include "echar/echar.h"
#include "key.h"
#include "score.h"

uint32_t stbrett_optimize_ve3nea(
    const struct echar var[26],
    struct Key* ckey,
    int len,
    const enigma_score_function_t* scoring
);
