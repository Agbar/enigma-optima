#pragma once

#include "score.h"

int stbrett_optimize_krah(
    struct echar var[26],
    struct Key* ckey,
    int len,
    const enigma_score_function_t* scoring
);