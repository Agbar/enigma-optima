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

/* declaration of internal functions */
void enigma_score_function_copy(enigma_score_function_t* to, const enigma_score_function_t* from);

// default scores
static double icscore(const decode_mapping_t* stbrett, int len);
static int uniscore(const decode_mapping_t* stbrett,  int len);
static int biscore(const decode_mapping_t* stbrett, int len);
static int triscore(const decode_mapping_t* stbrett, int len);

enigma_score_function_t enigma_score_orig;
enigma_score_function_t enigma_score_opt    = { triscore, biscore, icscore, uniscore };


typedef struct _enigma_score_testing_t
{
    enigma_score_function_t functions;
    enigma_score_function_t* reference;
    enigma_score_function_t* tested;
} enigma_score_testing_t ;

/** \brief Defines function used to testing one implementation of score function against the other.
 *
 * \param TESTING_FUNCTION_NAME Name of function implemented by this macro.
 * \param TESTING_OBJECT_INSTANCE instance of enigma_score_testing_t used by implementation.
 * \param FUNCTION_NAME tested function selected from enigma_score_function_t (triscore, biscore...).
 * \param RETURN_T type returned by tested function: int or double.
 *
 */
#define SCORE_TESTING_FUNCTION_ALWAYS_TEST_BOTH(TESTING_FUNCTION_NAME, TESTING_OBJECT_INSTANCE, FUNCTION_NAME, RETURN_T)\
static RETURN_T TESTING_FUNCTION_NAME(const decode_mapping_t* stbrett, int len)\
{\
    RETURN_T score = TESTING_OBJECT_INSTANCE.tested->FUNCTION_NAME(stbrett, len);\
    RETURN_T reference_score = TESTING_OBJECT_INSTANCE.reference->FUNCTION_NAME(stbrett, len);\
    if ( score != reference_score )\
    {\
    }\
    return reference_score;\
}

#define SCORE_TESTING_FUNCTION(NAME_PREFIX,NAME_SUFFIX,IMPL_MACRO )\
extern enigma_score_testing_t NAME_PREFIX##NAME_SUFFIX;\
\
IMPL_MACRO(NAME_PREFIX##_triscore##NAME_SUFFIX, CCAT(NAME_PREFIX,NAME_SUFFIX), triscore, int)\
IMPL_MACRO(NAME_PREFIX## _biscore##NAME_SUFFIX, CCAT(NAME_PREFIX,NAME_SUFFIX),  biscore, int)\
IMPL_MACRO(NAME_PREFIX## _icscore##NAME_SUFFIX, CCAT(NAME_PREFIX,NAME_SUFFIX),  icscore, double)\
IMPL_MACRO(NAME_PREFIX##_uniscore##NAME_SUFFIX, CCAT(NAME_PREFIX,NAME_SUFFIX), uniscore, int)\
\
enigma_score_testing_t NAME_PREFIX##NAME_SUFFIX = {\
    {\
        NAME_PREFIX##_triscore##NAME_SUFFIX,\
        NAME_PREFIX## _biscore##NAME_SUFFIX,\
        NAME_PREFIX## _icscore##NAME_SUFFIX,\
        NAME_PREFIX##_uniscore##NAME_SUFFIX,\
    },\
    0,0\
};

#define SCORE_TESTING_PREFIX enigma_score_testing
#define CCAT(A,B) A##B

SCORE_TESTING_FUNCTION(enigma_score_testing, _always_both, SCORE_TESTING_FUNCTION_ALWAYS_TEST_BOTH);

inline
void enigma_score_function_copy(enigma_score_function_t* to, const enigma_score_function_t* prototype)
{
    to->triscore = prototype->triscore;
    to->biscore  = prototype->biscore;
    to->icscore  = prototype->icscore;
    to->uniscore = prototype->uniscore;
}

//inline
void enigma_score_testing_create(enigma_score_testing_t* this, enigma_score_function_t* reference, enigma_score_function_t* tested)
{
    // this->funciotns is set up automagically by macro
    this->reference = reference;
    this->tested  = tested;
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
        enigma_score_testing_create(&CCAT(SCORE_TESTING_PREFIX,_always_both), &enigma_score_opt, &enigma_score_ssse3);
        enigma_score_function_copy(sf,&CCAT(SCORE_TESTING_PREFIX,_always_both).functions);
# endif
    }
}


/*
 * opti scores
 ************************/
__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static double icscore(const decode_mapping_t* stbrett, int len)
{
  int f[26] = {0};
  int S0, S1, S2, S3;
  text_t c1;
  int i;

  if (len < 2)
    return 0;

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

static int uniscore(const decode_mapping_t* stbrett, int len)
{
  int i;
  text_t c;
  int s;

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
int biscore(const decode_mapping_t* stbrett, int len)
{
  int i;
  text_t c1, c2;
  int s = 0;

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
int triscore(const decode_mapping_t* stbrett, int len)
{
  int i;
  text_t c1, c2, c3;
  int s;

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

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
