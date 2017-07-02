#ifndef STECKER_H
#define STECKER_H

#include "key.h"
#include "config/types.h"

void get_stecker(Key *key);
void rand_var(text_t var[]);
void set_to_ct_freq(text_t var[], int len);

/** \brief Swaps letters i and k
 *
 * \param key Key*
 * \param i int letter to swap with k
 * \param k int Letter to swap with i
 * \return void
 */
inline
void SwapStbrett(Key* const key, int i, int k) {
  text_t store = key->stbrett.letters[i];
  key->stbrett.letters[i] = key->stbrett.letters[k];
  key->stbrett.letters[k] = store;
}

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
