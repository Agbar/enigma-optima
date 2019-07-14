#pragma once

#include "echar/echar.h"
#include "key.h"
#include "score.h"

int stbrett_optimize_krah(
    const struct echar var[26],
    struct Key* ckey,
    int len,
    const enigma_score_function_t* scoring
);