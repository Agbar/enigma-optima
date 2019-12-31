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

#include "state.h"
#include "stbrett/optimizer.h"


struct HillclimbersKnapsack {
    stbrett_optimize_f* optimizer;
    void (*save_state)( const struct State* state, bool force_save );
};


void hillclimb( struct State *state,
                int max_pass, bool resume,
                FILE *outfile, int len,
                const struct HillclimbersKnapsack* knapsack );


bool check_knapsack( const struct HillclimbersKnapsack* knapsack );
