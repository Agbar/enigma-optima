#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "config/types.h"
#include "character_encoding.h"
#include "echar/echar.h"
#include "global.h"
#include "key.h"

struct State {
    const struct Key *const from;
    const struct Key *const to;
    struct Key *const ckey;
    struct Key *const gkey;
    int sw_mode;
    int pass;
    bool firstpass;
    uint32_t max_score;
    const struct echar * const ciphertext;
};

void save_state( const struct State* state );

NO_RETURN 
void save_state_exit( const struct State* state, int retval );

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
