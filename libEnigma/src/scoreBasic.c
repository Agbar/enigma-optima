#include <stdbool.h>
#include <string.h>
#include "cpu.h"
#include "cipher.h"
#include "dict.h"
#include "key.h"
#include "score.h"
#include "ciphertext.h"
#include "config/array_sizes.h"
#include "config/types.h"

// default scores
static uint16_t icscoreBasic( const Key* const restrict key, scoreLength_t length );
static int     uniscoreBasic( const Key* const restrict key, scoreLength_t length );
static int      biscoreBasic( const Key* const restrict key, scoreLength_t length );
static int     triscoreBasic( const Key* const restrict key, scoreLength_t length );

enigma_score_function_t enigmaScoreBasic = { triscoreBasic, biscoreBasic, icscoreBasic, uniscoreBasic };

union ScoringDecodedMessage decodedMsgPartBasic;

/*
 * opti scores
 ************************/
__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static uint16_t icscoreBasic( const Key* const restrict key, scoreLength_t len )
{
  int f[26] = {0};
  int i;

  if (len < 2)
    return 0;

  const PermutationMap_t* stbrett = &key->stbrett;

  for (i = 0; i < len-15; i += 16) {
    v4qs c;
    c = decode4(0,i,stbrett);
    f[c[0]]++;
    f[c[1]]++;
    f[c[2]]++;
    f[c[3]]++;

    c = decode4(4,i,stbrett);
    f[c[0]]++;
    f[c[1]]++;
    f[c[2]]++;
    f[c[3]]++;

    c = decode4(8,i,stbrett);
    f[c[0]]++;
    f[c[1]]++;
    f[c[2]]++;
    f[c[3]]++;

    c = decode4(12,i,stbrett);
    f[c[0]]++;
    f[c[1]]++;
    f[c[2]]++;
    f[c[3]]++;
  }
  for (; i < len-3; i += 4) {
    v4qs c = decode4(0,i,stbrett);
    f[c[0]]++;
    f[c[1]]++;
    f[c[2]]++;
    f[c[3]]++;
  }
  for (; i < len; i++) {
    text_t c1 = decode(0,i,stbrett);
    f[c1]++;
  }

  uint16_t S0, S1, S2, S3;
  S0 = S1 = S2 = S3 = 0;
  for (i = 0; i < 23; i += 4) {
    S0 += f[i]*(f[i]-1);
    S1 += f[i+1]*(f[i+1]-1);
    S2 += f[i+2]*(f[i+2]-1);
    S3 += f[i+3]*(f[i+3]-1);
  }
  S0 += f[24]*(f[24]-1);
  S1 += f[25]*(f[25]-1);

  return (S0+S1) + (S2+S3);
}

#define UNISCORE_ADD(S,A)\
    asm( "add  %1, %0": "+q"( (S) ): "m"( unidict[(A)] ) )
static int uniscoreBasic( const Key* key, scoreLength_t len )
{
  int i;
  text_t c;
  int s;

  const PermutationMap_t* stbrett = &key->stbrett;

  s = 0;
  for (i = 0; i < len-15; i += 16) {
    c = decode(0,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(1,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(2,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(3,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(4,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(5,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(6,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(7,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(8,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(9,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(10,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(11,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(12,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(13,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(14,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(15,i,stbrett);
    UNISCORE_ADD( s, c );
  }
  for (; i < len-3; i += 4) {
    c = decode(0,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(1,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(2,i,stbrett);
    UNISCORE_ADD( s, c );

    c = decode(3,i,stbrett);
    UNISCORE_ADD( s, c );
  }
  for (; i < len; i++) {
    c = decode(0,i,stbrett);
    UNISCORE_ADD( s, c );
  }

  return s;

}

#define BISCORE_ADD(S,A,B)\
    asm( "add  %1, %0": "+q"( (S) ): "m"( bidict[(A)][(B)] ) )
__attribute__ ((optimize("sched-stalled-insns=0"
                        ",sched-stalled-insns-dep=16")))
int biscoreBasic( const Key* const restrict key, scoreLength_t len )
{
    const PermutationMap_t* const stbrett = &key->stbrett;
    int s = 0;

    size_t c1 = decode(0,0,stbrett);

    int i = 1;
    for( ; i < len - 15; i += 16 ) {
        size_t c2 = decode( 0, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 1, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 2, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 3, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 4, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 5, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 6, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 7, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 8, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 9, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 10, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 11, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 12, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 13, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 14, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 15, i, stbrett );
        BISCORE_ADD( s , c2, c1 );
    }
    for( ; i < len - 3; i += 4 ) {
        size_t c2 = decode( 0, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 1, i, stbrett );
        BISCORE_ADD( s , c2, c1 );

        c2 = decode( 2, i, stbrett );
        BISCORE_ADD( s , c1, c2 );

        c1 = decode( 3, i, stbrett );
        BISCORE_ADD( s , c2, c1 );
    }
    for( ; i < len; i++ ) {
        size_t c2 = decode( 0, i, stbrett );
        BISCORE_ADD( s , c1, c2 );
        c1 = c2;
    }
    return s;
}

#define TRISCORE_ADD(S,A,B,C) \
    asm( "add  %1, %0": "+q"( (s) ): "m"( tridict[(A)][(B)][(C)] ) )

__attribute__ ((optimize("sched-stalled-insns=0"
                         ",sched-stalled-insns-dep=16"
               )))
int triscoreBasic( const Key* const restrict key, scoreLength_t len )
{
    int s = 0;

    const PermutationMap_t* const stbrett = &key->stbrett;

    size_t c1 = decode(0,0,stbrett);
    size_t c2 = decode(1,0,stbrett);

    int i = 2;
    for( ; i < len - 15; i += 16 ) {
        size_t c3;
        c3 = decode( 0, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = decode( 1, i, stbrett );
        TRISCORE_ADD( s, c2, c3, c1 );

        c2 = decode( 2, i, stbrett );
        TRISCORE_ADD( s, c3, c1, c2 );

        c3 = decode( 3, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = decode( 4, i, stbrett );
        TRISCORE_ADD( s, c2, c3, c1 );

        c2 = decode( 5, i, stbrett );
        TRISCORE_ADD( s, c3, c1, c2 );

        c3 = decode( 6, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = decode( 7, i, stbrett );
        TRISCORE_ADD( s, c2, c3, c1 );

        c2 = decode( 8, i, stbrett );
        TRISCORE_ADD( s, c3, c1, c2 );

        c3 = decode( 9, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = decode( 10, i, stbrett );
        TRISCORE_ADD( s, c2, c3, c1 );

        c2 = decode( 11, i, stbrett );
        TRISCORE_ADD( s, c3, c1, c2 );

        c3 = decode( 12, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = decode( 13, i, stbrett );
        TRISCORE_ADD( s, c2, c3, c1 );

        c2 = decode( 14, i, stbrett );
        TRISCORE_ADD( s, c3, c1, c2 );

        c3 = decode( 15, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = c2;
        c2 = c3;
    }
    for( ; i < len - 3; i += 4 ) {
        size_t c3;
        c3 = decode( 0, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = decode( 1, i, stbrett );
        TRISCORE_ADD( s, c2, c3, c1 );

        c2 = decode( 2, i, stbrett );
        TRISCORE_ADD( s, c3, c1, c2 );

        c3 = decode( 3, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

        c1 = c2;
        c2 = c3;
    }
    for( ; i < len; ++i ) {
        size_t c3;
        c3 = decode( 0, i, stbrett );
        TRISCORE_ADD( s, c1, c2, c3 );

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
