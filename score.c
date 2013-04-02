#include "cipher.h"
#include "key.h"
#include "score.h"

extern int tridict[][32][32];
extern int bidict[][32];
extern int unidict[26];
extern int path_lookup[][32];

const size_t d = 32; //last dimension size

/* returns the trigram score of a key/ciphertext combination */
int get_triscore(const Key *key, const int *ciphertext, int len)
{
  init_path_lookup_ALL(key, len);
  return triscore(key->stbrett, ciphertext, len);
}

#ifdef SIMPLESCORE
double icscore(const int *stbrett, const int *ciphertext, int len)
{
  int f[26] = {0};
  double S = 0;
  int i;
  int c;

  if (len < 2)
    return 0;

  for (i = 0; i < len; i++) {
    c = stbrett[ciphertext[i]];
    c = path_lookup[i][c];
    c = stbrett[c];
    f[c]++;
  }

  for (i = 0; i < 26; i++)
    S += f[i]*(f[i]-1);
  S /= len*(len-1);

  return S;
}


int uniscore(const int *stbrett, const int *ciphertext, int len)
{
  int i;
  int c;
  int s = 0;

  for (i = 0; i < len; i++) {
    c = stbrett[ciphertext[i]];
    c = path_lookup[i][c];
    c = stbrett[c];
    s += unidict[c];
  }

  return s;
}


int biscore(const int *stbrett, const int *ciphertext, int len)
{
  int i;
  int c1, c2;
  int s = 0;

  c1 = stbrett[ciphertext[0]];
  c1 = path_lookup[0][c1];
  c1 = stbrett[c1];

  for (i = 1; i < len; i++) {
    c2 = stbrett[ciphertext[i]];
    c2 = path_lookup[i][c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

int triscore(const int *stbrett, const int *ciphertext, int len)
{
  int i;
  int c1, c2, c3;
  int s = 0;

  c1 = stbrett[ciphertext[0]];
  c1 = path_lookup[0][c1];
  c1 = stbrett[c1];

  c2 = stbrett[ciphertext[1]];
  c2 = path_lookup[1][c2];
  c2 = stbrett[c2];

  for (i = 2; i < len; i++) {
    c3 = stbrett[ciphertext[i]];
    c3 = path_lookup[i][c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }

  return s;
}

#endif


#ifndef SIMPLESCORE
double icscore(const int *stbrett, const int *ciphertext, int len)
{
  int f[26] = {0};
  int S0, S1, S2, S3;
  int c1;
  int i;

  if (len < 2)
    return 0;

  for (i = 0; i < len-15; i += 16) {
    c1 = stbrett[ciphertext[i]];
    c1 = path_lookup[i][c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+1]];
    c1 = path_lookup[1][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+2]];
    c1 = path_lookup[2][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+3]];
    c1 = path_lookup[3][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+4]];
    c1 = path_lookup[4][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+5]];
    c1 = path_lookup[5][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+6]];
    c1 = path_lookup[6][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+7]];
    c1 = path_lookup[7][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+8]];
    c1 = path_lookup[8][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+9]];
    c1 = path_lookup[9][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+10]];
    c1 = path_lookup[10][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+11]];
    c1 = path_lookup[11][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+12]];
    c1 = path_lookup[12][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+13]];
    c1 = path_lookup[13][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+14]];
    c1 = path_lookup[14][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+15]];
    c1 = path_lookup[15][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;
  }
  for (; i < len-3; i += 4) {
    c1 = stbrett[ciphertext[i]];
    c1 = path_lookup[i][c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+1]];
    c1 = path_lookup[1][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+2]];
    c1 = path_lookup[2][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;

    c1 = stbrett[ciphertext[i+3]];
    c1 = path_lookup[3][i*d+c1];
    c1 = stbrett[c1];
    f[c1]++;
  }
  for (; i < len; i++) {
    c1 = stbrett[ciphertext[i]];
    c1 = path_lookup[i][c1];
    c1 = stbrett[c1];
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

int uniscore(const int *stbrett, const int *ciphertext, int len)
{
  int i;
  int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16;
  int s0, s1, s2, s3;


  s0 = s1 = s2 = s3 = 0;
  for (i = 0; i < len-15; i += 16) {
    c1 = stbrett[ciphertext[i]];
    c1 = path_lookup[i][c1];
    c1 = stbrett[c1];
    s0 += unidict[c1];

    c2 = stbrett[ciphertext[i+1]];
    c2 = path_lookup[i+1][c2];
    c2 = stbrett[c2];
    s1 += unidict[c2];

    c3 = stbrett[ciphertext[i+2]];
    c3 = path_lookup[i+2][c3];
    c3 = stbrett[c3];
    s2 += unidict[c3];

    c4 = stbrett[ciphertext[i+3]];
    c4 = path_lookup[i+3][c4];
    c4 = stbrett[c4];
    s3 += unidict[c4];

    c5 = stbrett[ciphertext[i+4]];
    c5 = path_lookup[i+4][c5];
    c5 = stbrett[c5];
    s0 += unidict[c5];

    c6 = stbrett[ciphertext[i+5]];
    c6 = path_lookup[i+5][c6];
    c6 = stbrett[c6];
    s1 += unidict[c6];

    c7 = stbrett[ciphertext[i+6]];
    c7 = path_lookup[i+6][c7];
    c7 = stbrett[c7];
    s2 += unidict[c7];

    c8 = stbrett[ciphertext[i+7]];
    c8 = path_lookup[i+7][c8];
    c8 = stbrett[c8];
    s3 += unidict[c8];

    c9 = stbrett[ciphertext[i+8]];
    c9 = path_lookup[i+8][c9];
    c9 = stbrett[c9];
    s0 += unidict[c9];

    c10 = stbrett[ciphertext[i+9]];
    c10 = path_lookup[i+9][c10];
    c10 = stbrett[c10];
    s1 += unidict[c10];

    c11 = stbrett[ciphertext[i+10]];
    c11 = path_lookup[i+10][c11];
    c11 = stbrett[c11];
    s2 += unidict[c11];

    c12 = stbrett[ciphertext[i+11]];
    c12 = path_lookup[i+11][c12];
    c12 = stbrett[c12];
    s3 += unidict[c12];

    c13 = stbrett[ciphertext[i+12]];
    c13 = path_lookup[i+12][c13];
    c13 = stbrett[c13];
    s0 += unidict[c13];

    c14 = stbrett[ciphertext[i+13]];
    c14 = path_lookup[i+13][c14];
    c14 = stbrett[c14];
    s1 += unidict[c14];

    c15 = stbrett[ciphertext[i+14]];
    c15 = path_lookup[i+14][c15];
    c15 = stbrett[c15];
    s2 += unidict[c15];

    c16 = stbrett[ciphertext[i+15]];
    c16 = path_lookup[i+15][c16];
    c16 = stbrett[c16];
    s3 += unidict[c16];
  }
  for (; i < len-3; i += 4) {
    c1 = stbrett[ciphertext[i]];
    c1 = path_lookup[i][c1];
    c1 = stbrett[c1];
    s0 += unidict[c1];

    c2 = stbrett[ciphertext[i+1]];
    c2 = path_lookup[i+1][c2];
    c2 = stbrett[c2];
    s1 += unidict[c2];

    c3 = stbrett[ciphertext[i+2]];
    c3 = path_lookup[i+2][c3];
    c3 = stbrett[c3];
    s2 += unidict[c3];

    c4 = stbrett[ciphertext[i+3]];
    c4 = path_lookup[i+3][c4];
    c4 = stbrett[c4];
    s3 += unidict[c4];
  }
  for (; i < len; i++) {
    c1 = stbrett[ciphertext[i]];
    c1 = path_lookup[i][c1];
    c1 = stbrett[c1];
    s0 += unidict[c1];
  }

  return (s0+s1) + (s2+s3);

}


int biscore(const int *stbrett, const int *ciphertext, int len)
{
  int i;
  int c1, c2;
  int s = 0;

  c1 = stbrett[ciphertext[0]];
  c1 = path_lookup[0][c1];
  c1 = stbrett[c1];

  for (i = 1; i < len-15; i += 16) {
    c2 = stbrett[ciphertext[i]];
    c2 = path_lookup[i][c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+1]];
    c1 = path_lookup[1][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+2]];
    c2 = path_lookup[2][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+3]];
    c1 = path_lookup[3][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+4]];
    c2 = path_lookup[4][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+5]];
    c1 = path_lookup[5][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+6]];
    c2 = path_lookup[6][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+7]];
    c1 = path_lookup[7][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+8]];
    c2 = path_lookup[8][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+9]];
    c1 = path_lookup[9][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+10]];
    c2 = path_lookup[10][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+11]];
    c1 = path_lookup[11][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+12]];
    c2 = path_lookup[12][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+13]];
    c1 = path_lookup[13][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+14]];
    c2 = path_lookup[14][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+15]];
    c1 = path_lookup[15][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];
  }
  for (; i < len-3; i += 4) {
    c2 = stbrett[ciphertext[i]];
    c2 = path_lookup[i][c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+1]];
    c1 = path_lookup[1][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];

    c2 = stbrett[ciphertext[i+2]];
    c2 = path_lookup[2][i*d+c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = stbrett[ciphertext[i+3]];
    c1 = path_lookup[3][i*d+c1];
    c1 = stbrett[c1];
    s += bidict[c2][c1];
  }
  for (; i < len; i++) {
    c2 = stbrett[ciphertext[i]];
    c2 = path_lookup[i][c2];
    c2 = stbrett[c2];
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

int triscore(const int *stbrett, const int *ciphertext, int len)
{
  int i;
  int c1, c2, c3;
  int s;

  s=0;

  c1 = stbrett[ciphertext[0]];
  c1 = path_lookup[0][c1];
  c1 = stbrett[c1];

  c2 = stbrett[ciphertext[1]];
  c2 = path_lookup[1][c2];
  c2 = stbrett[c2];

  for (i = 2; i < len-15; i += 16) {
    c3 = stbrett[ciphertext[i]];
    c3 = path_lookup[0][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = stbrett[ciphertext[i+1]];
    c1 = path_lookup[1][i*d+c1];
    c1 = stbrett[c1];
    s += tridict[c2][c3][c1];

    c2 = stbrett[ciphertext[i+2]];
    c2 = path_lookup[2][i*d+c2];
    c2 = stbrett[c2];
    s += tridict[c3][c1][c2];

    c3 = stbrett[ciphertext[i+3]];
    c3 = path_lookup[3][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = stbrett[ciphertext[i+4]];
    c1 = path_lookup[4][i*d+c1];
    c1 = stbrett[c1];
    s += tridict[c2][c3][c1];

    c2 = stbrett[ciphertext[i+5]];
    c2 = path_lookup[5][i*d+c2];
    c2 = stbrett[c2];
    s += tridict[c3][c1][c2];

    c3 = stbrett[ciphertext[i+6]];
    c3 = path_lookup[6][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = stbrett[ciphertext[i+7]];
    c1 = path_lookup[7][i*d+c1];
    c1 = stbrett[c1];
    s += tridict[c2][c3][c1];

    c2 = stbrett[ciphertext[i+8]];
    c2 = path_lookup[8][i*d+c2];
    c2 = stbrett[c2];
    s += tridict[c3][c1][c2];

    c3 = stbrett[ciphertext[i+9]];
    c3 = path_lookup[9][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = stbrett[ciphertext[i+10]];
    c1 = path_lookup[10][i*d+c1];
    c1 = stbrett[c1];
    s += tridict[c2][c3][c1];

    c2 = stbrett[ciphertext[i+11]];
    c2 = path_lookup[11][i*d+c2];
    c2 = stbrett[c2];
    s += tridict[c3][c1][c2];

    c3 = stbrett[ciphertext[i+12]];
    c3 = path_lookup[12][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = stbrett[ciphertext[i+13]];
    c1 = path_lookup[13][i*d+c1];
    c1 = stbrett[c1];
    s += tridict[c2][c3][c1];

    c2 = stbrett[ciphertext[i+14]];
    c2 = path_lookup[14][i*d+c2];
    c2 = stbrett[c2];
    s += tridict[c3][c1][c2];

    c3 = stbrett[ciphertext[i+15]];
    c3 = path_lookup[15][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }
  for (; i < len-3; i += 4) {
    c3 = stbrett[ciphertext[i]];
    c3 = path_lookup[i][c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = stbrett[ciphertext[i+1]];
    c1 = path_lookup[1][i*d+c1];
    c1 = stbrett[c1];
    s += tridict[c2][c3][c1];

    c2 = stbrett[ciphertext[i+2]];
    c2 = path_lookup[2][i*d+c2];
    c2 = stbrett[c2];
    s += tridict[c3][c1][c2];

    c3 = stbrett[ciphertext[i+3]];
    c3 = path_lookup[3][i*d+c3];
    c3 = stbrett[c3];
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }
  for (; i < len; i++) {
    c3 = stbrett[ciphertext[i]];
    c3 = path_lookup[i][c3];
    c3 = stbrett[c3];
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
