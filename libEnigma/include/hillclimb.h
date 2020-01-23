#pragma once
/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */

#include <stdbool.h>
#include <stdio.h>

#include "key.h"
#include "state.h"
#include "stbrett/optimizer.h"


struct ScoreOptimizer {
    stbrett_optimize_f* optimize_score;
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup;
    enigma_score_function_t* score_impl;
};


struct HillclimbersKnapsack {
    struct ScoreOptimizer* optimizer;
    void ( *on_new_best )( const struct Key* gkey, int len );
    void (*save_state)( const struct State* state, bool force_save );
    void ( *log )( const char msg[] );
    bool ( *scrambler_state_is_endloop )( const struct State* state, const struct Key* ckey, int len );
};


void hillclimb( struct State *state,
                int max_pass,
                int len,
                const struct HillclimbersKnapsack* knapsack );


bool check_knapsack( const struct HillclimbersKnapsack* knapsack );


bool check_scrambler_state_is_endloop( const struct State* state,
                                       const struct Key* ckey,
                                       int len );
