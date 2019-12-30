#pragma once

#include "config/types.h"
#include "character_encoding.h"
#include "global.h"

struct State {
  const struct Key *from;
  const struct Key *to;
  struct Key *ckey;
  struct Key *gkey;
  int *sw_mode;
  int *pass;
  int *firstpass;
  int *max_score;
  struct echar *ciphertext;
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
