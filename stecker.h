#ifndef STECKER_H
#define STECKER_H

#include <stdint.h>

#include "key.h"
#include "config\types.h"

void get_stecker(Key *key);
void rand_var(text_t var[]);
void set_to_ct_freq(text_t var[], int len);

/* swaps letters */
inline void swap(PermutationMap_t *stbrett, int i, int k)
{
  text_t store;

  store = stbrett->letters[i];
  stbrett->letters[i] = stbrett->letters[k];
  stbrett->letters[k] = store;
}

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
