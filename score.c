#include "cipher.h"
#include "key.h"
#include "score.h"
#include "config\array_sizes.h"
#include "config\types.h"

extern dict_t tridict[][LAST_DIMENSION][LAST_DIMENSION];
extern dict_t bidict[][LAST_DIMENSION];
extern dict_t unidict[26];
extern text_t path_lookup[][LAST_DIMENSION];

const size_t d = LAST_DIMENSION; //last dimension size

/* returns the trigram score of a key/ciphertext combination */
int get_triscore(const Key *key, const text_t *ciphertext, int len)
{
  init_path_lookup_ALL(key, len);
  return triscore(key->stbrett, ciphertext, len);
}

#ifdef SIMPLESCORE
double icscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int f[26] = {0};
  double S = 0;
  int i;
  int c;

  if (len < 2)
    return 0;

  for (i = 0; i < len; i++) {
    DECODE(c,0,i);
    s += unidict[c];
    f[c]++;
  }

  for (i = 0; i < 26; i++)
    S += f[i]*(f[i]-1);
  S /= len*(len-1);

  return S;
}


int uniscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int i;
  int c;
  int s = 0;

  for (i = 0; i < len; i++) {
    DECODE(c,0,i);
    s += unidict[c];
  }

  return s;
}


int biscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int i;
  int c1, c2;
  int s = 0;

  DECODE(c1,0,i);


  for (i = 1; i < len; i++) {
    DECODE(c2,0,i);
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

int triscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int i;
  int c1, c2, c3;
  int s = 0;

  DECODE(c1,0,i);

  DECODE(c2,0,i);

  for (i = 2; i < len; i++) {
    DECODE(c3,0,i);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }

  return s;
}

#endif


#ifndef SIMPLESCORE
double icscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int f[26] = {0};
  int S0, S1, S2, S3;
  text_t c1;
  int i;

  if (len < 2)
    return 0;

  for (i = 0; i < len-15; i += 16) {
    DECODE(c1,0,i);
    f[c1]++;

    DECODE(c1,1,i);
    f[c1]++;

    DECODE(c1,2,i);
    f[c1]++;

    DECODE(c1,3,i);
    f[c1]++;

    DECODE(c1,4,i);
    f[c1]++;

    DECODE(c1,5,i);
    f[c1]++;

    DECODE(c1,6,i);
    f[c1]++;

    DECODE(c1,7,i);
    f[c1]++;

    DECODE(c1,8,i);
    f[c1]++;

    DECODE(c1,9,i);
    f[c1]++;

    DECODE(c1,10,i);
    f[c1]++;

    DECODE(c1,11,i);
    f[c1]++;

    DECODE(c1,12,i);
    f[c1]++;

    DECODE(c1,13,i);
    f[c1]++;

    DECODE(c1,14,i);
    f[c1]++;

    DECODE(c1,15,i);
    f[c1]++;
  }
  for (; i < len-3; i += 4) {
    DECODE(c1,0,i);
    f[c1]++;

    DECODE(c1,1,i);
    f[c1]++;

    DECODE(c1,2,i);
    f[c1]++;

    DECODE(c1,3,i);
    f[c1]++;
  }
  for (; i < len; i++) {
    DECODE(c1,0,i);
    f[c1]++;
  }


  S0 = S1 = S2 = S3 = 0;
  for (i = 0; i < 23; i += 4) {
    S0 += f[i]*(f[i]-1);
    S1 += f[i+1]*(f[i+1]-1);
    S2 += f[i+2]*(f[i+2]-1);
    S3 += f[i+3]*(f[i+3]-1);
  }
  S0 += f[24]*(f[24]-1);
  S1 += f[25]*(f[25]-1);

  return (double)((S0+S1) + (S2+S3)) / (len*(len-1));

}

int uniscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int i;
  text_t c;
  int s;

  s = 0;
  for (i = 0; i < len-15; i += 16) {
    DECODE(c,0,i);
    s += unidict[c];

    DECODE(c,1,i);
    s += unidict[c];

    DECODE(c,2,i);
    s += unidict[c];

    DECODE(c,3,i);
    s += unidict[c];

    DECODE(c,4,i);
    s += unidict[c];

    DECODE(c,5,i);
    s += unidict[c];

    DECODE(c,6,i);
    s += unidict[c];

    DECODE(c,7,i);
    s += unidict[c];

    DECODE(c,8,i);
    s += unidict[c];

    DECODE(c,9,i);
    s += unidict[c];

    DECODE(c,10,i);
    s += unidict[c];

    DECODE(c,11,i);
    s += unidict[c];

    DECODE(c,12,i);
    s += unidict[c];

    DECODE(c,13,i);
    s += unidict[c];

    DECODE(c,14,i);
    s += unidict[c];

    DECODE(c,15,i);
    s += unidict[c];
  }
  for (; i < len-3; i += 4) {
    DECODE(c,0,i);
    s += unidict[c];

    DECODE(c,1,i);
    s += unidict[c];

    DECODE(c,2,i);
    s += unidict[c];

    DECODE(c,3,i);
    s += unidict[c];
  }
  for (; i < len; i++) {
    DECODE(c,0,i);
    s += unidict[c];
  }

  return s;

}


int biscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int i;
  text_t c1, c2;
  int s = 0;

  DECODE(c1,0,0)

  for (i = 1; i < len-15; i += 16) {
    DECODE(c2,0,i);
    s += bidict[c1][c2];

    DECODE(c1,1,i);
    s += bidict[c2][c1];

    DECODE(c2,2,i);
    s += bidict[c1][c2];

    DECODE(c1,3,i);
    s += bidict[c2][c1];

    DECODE(c2,4,i);
    s += bidict[c1][c2];

    DECODE(c1,5,i);
    s += bidict[c2][c1];

    DECODE(c2,6,i);
    s += bidict[c1][c2];

    DECODE(c1,7,i);
    s += bidict[c2][c1];

    DECODE(c2,8,i);
    s += bidict[c1][c2];

    DECODE(c1,9,i);
    s += bidict[c2][c1];

    DECODE(c2,10,i);
    s += bidict[c1][c2];

    DECODE(c1,11,i);
    s += bidict[c2][c1];

    DECODE(c2,12,i);
    s += bidict[c1][c2];

    DECODE(c1,13,i);
    s += bidict[c2][c1];

    DECODE(c2,14,i);
    s += bidict[c1][c2];

    DECODE(c1,15,i);
    s += bidict[c2][c1];
  }
  for (; i < len-3; i += 4) {
    DECODE(c2,0,i);
    s += bidict[c1][c2];

    DECODE(c1,1,i);
    s += bidict[c2][c1];

    DECODE(c2,2,i);
    s += bidict[c1][c2];

    DECODE(c1,3,i);
    s += bidict[c2][c1];
  }
  for (; i < len; i++) {
    DECODE(c2,0,i);
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

int triscore(const text_t *stbrett, const text_t *ciphertext, int len)
{
  int i;
  text_t c1, c2, c3;
  int s;

  s=0;

  DECODE(c1,0,0);

  DECODE(c2,1,0);

  for (i = 2; i < len-15; i += 16) {
    DECODE(c3,0,i);
    s += tridict[c1][c2][c3];

    DECODE(c1,1,i);
    s += tridict[c2][c3][c1];

    DECODE(c2,2,i);
    s += tridict[c3][c1][c2];

    DECODE(c3,3,i);
    s += tridict[c1][c2][c3];

    DECODE(c1,4,i);
    s += tridict[c2][c3][c1];

    DECODE(c2,5,i)
    s += tridict[c3][c1][c2];

    DECODE(c3,6,i);
    s += tridict[c1][c2][c3];

    DECODE(c1,7,i);
    s += tridict[c2][c3][c1];

    DECODE(c2,8,i);
    s += tridict[c3][c1][c2];

    DECODE(c3,9,i);
    s += tridict[c1][c2][c3];

    DECODE(c1,10,i);
    s += tridict[c2][c3][c1];

    DECODE(c2,11,i);
    s += tridict[c3][c1][c2];

    DECODE(c3,12,i);
    s += tridict[c1][c2][c3];

    DECODE(c1,13,i);
    s += tridict[c2][c3][c1];

    DECODE(c2,14,i);
    s += tridict[c3][c1][c2];

    DECODE(c3,15,i);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }
  for (; i < len-3; i += 4) {
    DECODE(c3,0,i);
    s += tridict[c1][c2][c3];

    DECODE(c1,1,i);
    s += tridict[c2][c3][c1];

    DECODE(c2,2,i);
    s += tridict[c3][c1][c2];

    DECODE(c3,3,i);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }
  for (; i < len; i++) {
    DECODE(c3,0,i);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }

  return s;

}

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
