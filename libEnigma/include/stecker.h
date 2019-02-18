#pragma once

#include "key.h"
#include "config/types.h"
#include "character_encoding.h"

void get_stecker(Key *key);
void rand_var( struct echar var[26] );
void set_to_ct_freq( struct echar var[26], int len );

/** \brief Swaps letters i and k
 *
 * \param key Key*
 * \param i int letter to swap with k
 * \param k int Letter to swap with i
 * \return void
 */
static inline
void SwapStbrett(Key* const key, struct echar i, struct echar k) {
  struct echar store = key->stbrett.letters[ echar_0_based_index( i ) ];
  key->stbrett.letters[ echar_0_based_index( i ) ] = key->stbrett.letters[ echar_0_based_index( k ) ];
  key->stbrett.letters[ echar_0_based_index( k ) ] = store;
}

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
