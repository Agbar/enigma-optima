#include <stdbool.h>
#include <string.h>
#include "cpu.h"
#include "cipher.h"
#include "dict.h"
#include "key.h"
#include "score.h"
#include "ciphertext.h"
#include "config\array_sizes.h"
#include "config\types.h"

#include "score_simple.h"
#include "x86\score_ssse3.h"

#ifdef TESTING_SCORE
# include "ScoreTesting.h"
#endif

/* declaration of internal functions */
void enigma_score_function_copy(enigma_score_function_t* restrict to, const enigma_score_function_t* restrict from);

// default scores
static double icscore(const Key* const restrict key, int len);
static int   uniscore(const Key* const restrict key, int len);
static int    biscore(const Key* const restrict key, int len);
static int   triscore(const Key* const restrict key, int len);

enigma_score_function_t enigma_score_orig;
enigma_score_function_t enigma_score_opt    = { triscore, biscore, icscore, uniscore };

union ScoringDecodedMessage decodedMessageStandard_d;

inline
void enigma_score_function_copy(enigma_score_function_t* restrict to, const enigma_score_function_t* restrict prototype)
{
    to->triscore = prototype->triscore;
    to->biscore  = prototype->biscore;
    to->icscore  = prototype->icscore;
    to->uniscore = prototype->uniscore;
}

void enigma_score_init(enigma_cpu_flags_t cpu, enigma_score_function_t* sf)
{
#ifdef ENIGMA_CPU_SPECIFIC
    cpu = ENIGMA_CPU_SPECIFIC;
#endif // ENIGMA_CPU_SPECIFIC

    enigma_score_function_copy(sf,&enigma_score_opt);


    if (cpu & enigma_cpu_ssse3)
    {
        enigma_score_function_copy(sf,&enigma_score_ssse3);

# ifdef TESTING_SCORE
        enigma_score_function_t* test = enigma_score_testing_create( EnigmaSF_Optimized, EnigmaSF_SSSE3 );
        enigma_score_function_copy(sf,test);
# endif
    }
}

/*
 * opti scores
 ************************/
__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static double icscore(const Key* const restrict key, int len)
{
  int f[26] = {0};
  int S0, S1, S2, S3;
  text_t c1;
  int i;

  if (len < 2)
    return 0;

  const PermutationMap_t* stbrett = &key->stbrett;

  for (i = 0; i < len-15; i += 16) {
    c1 = decode(0,i,stbrett);
    f[c1]++;

    c1 = decode(1,i,stbrett);
    f[c1]++;

    c1 = decode(2,i,stbrett);
    f[c1]++;

    c1 = decode(3,i,stbrett);
    f[c1]++;

    c1 = decode(4,i,stbrett);
    f[c1]++;

    c1 = decode(5,i,stbrett);
    f[c1]++;

    c1 = decode(6,i,stbrett);
    f[c1]++;

    c1 = decode(7,i,stbrett);
    f[c1]++;

    c1 = decode(8,i,stbrett);
    f[c1]++;

    c1 = decode(9,i,stbrett);
    f[c1]++;

    c1 = decode(10,i,stbrett);
    f[c1]++;

    c1 = decode(11,i,stbrett);
    f[c1]++;

    c1 = decode(12,i,stbrett);
    f[c1]++;

    c1 = decode(13,i,stbrett);
    f[c1]++;

    c1 = decode(14,i,stbrett);
    f[c1]++;

    c1 = decode(15,i,stbrett);
    f[c1]++;
  }
  for (; i < len-3; i += 4) {
    c1 = decode(0,i,stbrett);
    f[c1]++;

    c1 = decode(1,i,stbrett);
    f[c1]++;

    c1 = decode(2,i,stbrett);
    f[c1]++;

    c1 = decode(3,i,stbrett);
    f[c1]++;
  }
  for (; i < len; i++) {
    c1 = decode(0,i,stbrett);
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

static int uniscore(const Key* key, int len)
{
  int i;
  text_t c;
  int s;

  const PermutationMap_t* stbrett = &key->stbrett;

  s = 0;
  for (i = 0; i < len-15; i += 16) {
    c = decode(0,i,stbrett);
    s += unidict[c];

    c = decode(1,i,stbrett);
    s += unidict[c];

    c = decode(2,i,stbrett);
    s += unidict[c];

    c = decode(3,i,stbrett);
    s += unidict[c];

    c = decode(4,i,stbrett);
    s += unidict[c];

    c = decode(5,i,stbrett);
    s += unidict[c];

    c = decode(6,i,stbrett);
    s += unidict[c];

    c = decode(7,i,stbrett);
    s += unidict[c];

    c = decode(8,i,stbrett);
    s += unidict[c];

    c = decode(9,i,stbrett);
    s += unidict[c];

    c = decode(10,i,stbrett);
    s += unidict[c];

    c = decode(11,i,stbrett);
    s += unidict[c];

    c = decode(12,i,stbrett);
    s += unidict[c];

    c = decode(13,i,stbrett);
    s += unidict[c];

    c = decode(14,i,stbrett);
    s += unidict[c];

    c = decode(15,i,stbrett);
    s += unidict[c];
  }
  for (; i < len-3; i += 4) {
    c = decode(0,i,stbrett);
    s += unidict[c];

    c = decode(1,i,stbrett);
    s += unidict[c];

    c = decode(2,i,stbrett);
    s += unidict[c];

    c = decode(3,i,stbrett);
    s += unidict[c];
  }
  for (; i < len; i++) {
    c = decode(0,i,stbrett);
    s += unidict[c];
  }

  return s;

}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
int biscore(const Key* const restrict key, int len)
{
  int i;
  text_t c1, c2;
  int s = 0;

  const PermutationMap_t* const stbrett = &key->stbrett;

  c1 = decode(0,0,stbrett);

  for (i = 1; i < len-15; i += 16) {
    c2 = decode(0,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(1,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(2,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(3,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(4,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(5,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(6,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(7,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(8,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(9,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(10,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(11,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(12,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(13,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(14,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(15,i,stbrett);
    s += bidict[c2][c1];
  }
  for (; i < len-3; i += 4) {
    c2 = decode(0,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(1,i,stbrett);
    s += bidict[c2][c1];

    c2 = decode(2,i,stbrett);
    s += bidict[c1][c2];

    c1 = decode(3,i,stbrett);
    s += bidict[c2][c1];
  }
  for (; i < len; i++) {
    c2 = decode(0,i,stbrett);
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
int triscore(const Key* const restrict key, int len)
{
  int i;
  text_t c1, c2, c3;
  int s;

  const PermutationMap_t* const stbrett = &key->stbrett;

  s=0;

  c1 = decode(0,0,stbrett);

  c2 = decode(1,0,stbrett);

  for (i = 2; i < len-15; i += 16) {
    c3 = decode(0,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = decode(1,i,stbrett);
    s += tridict[c2][c3][c1];

    c2 = decode(2,i,stbrett);
    s += tridict[c3][c1][c2];

    c3 = decode(3,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = decode(4,i,stbrett);
    s += tridict[c2][c3][c1];

    c2 = decode(5,i,stbrett);
    s += tridict[c3][c1][c2];

    c3 = decode(6,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = decode(7,i,stbrett);
    s += tridict[c2][c3][c1];

    c2 = decode(8,i,stbrett);
    s += tridict[c3][c1][c2];

    c3 = decode(9,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = decode(10,i,stbrett);
    s += tridict[c2][c3][c1];

    c2 = decode(11,i,stbrett);
    s += tridict[c3][c1][c2];

    c3 = decode(12,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = decode(13,i,stbrett);
    s += tridict[c2][c3][c1];

    c2 = decode(14,i,stbrett);
    s += tridict[c3][c1][c2];

    c3 = decode(15,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }
  for (; i < len-3; i += 4) {
    c3 = decode(0,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = decode(1,i,stbrett);
    s += tridict[c2][c3][c1];

    c2 = decode(2,i,stbrett);
    s += tridict[c3][c1][c2];

    c3 = decode(3,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }
  for (; i < len; i++) {
    c3 = decode(0,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }

  return s;

}

void DecodeScoredMessagePartStandard(const Key* const restrict key, int len, union ScoringDecodedMessage* restrict output){
    int i = 0;
    for( ; i < len; ++i ){
        output->plain[i] = decode( 0, i, &key->stbrett );
    }
}

bool GetDifferences( union ScoringDecodedMessage* restrict reference, union ScoringDecodedMessage* restrict tested, char* restrict output, int len ){
    char* ref = (char*)reference->plain;
    char* tes = (char*)tested->plain;
    int   i   = 0;
    bool  differenceSpotted = false;
    for( ; i < len; ++i ){
        if( ref[i] != tes[i] ){
            output[i] = '^';
            differenceSpotted = true;
        }
    }
    return differenceSpotted;
}

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
