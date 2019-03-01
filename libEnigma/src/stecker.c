#include  <stddef.h>

#include "stecker.h"

#include "ciphertext.h"
#include "randomNumbers.h"

/* extracts stecker from key->stbrett to key->sf */
void get_stecker( struct Key *const key )
{
  size_t i;
  int k = 25;

  key->count = 0;
  for (i = 0; i < 26; i++) {
    if ( echar_0_based_index( key->stbrett.letters[i] ) > i) {
      key->sf.map[key->count++] = make_echar( i );
      key->sf.map[key->count++] = key->stbrett.letters[i];
    }
    else if ( echar_0_based_index( key->stbrett.letters[i] ) == i) {
      key->sf.map[k--] =  make_echar( i );
    }
  }
}

/* get new order for testing stecker */
void rand_var( struct echar var[26] )
{
  int count;
  int i;

  for (count = 25; count > 0; count--) {
    i = GetRandomNumber() % ( count + 1 );
    struct echar store = var[count];
    var[count] = var[i];
    var[i] = store;
  }
}

/* arrange var[] in order of frequency of letters in ciphertext */
void set_to_ct_freq( struct echar var[26], int len )
{
  int f[26] = {0};
  int i, k;
  int max, pos = -1;
  int n = 0;

  for (i = 0; i < len; i++) {
    size_t c = echar_0_based_index( ciphertext.plain[i] );
    f[c]++;
  }

  for (i = 0; i < 26; i++) {
    max = 0;
    for (k = 0; k < 26; k++)
      if (f[k] >= max) {
        max = f[k];
        pos = k;
      }
    if (pos == -1)
      return;
    f[pos] = -1;
    var[n++] = make_echar( pos );
  }
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
