#include "dict.h"
#include "key.h"
#include "score.h"

// default scores
static int   uniscoreNoInterleave( const Key* const restrict key, int len );
static int    biscoreNoInterleave( const Key* const restrict key, int len );
static int   triscoreNoInterleave( const Key* const restrict key, int len );
static double icscoreNoInterleave( const Key* const restrict key, int len );

union ScoringDecodedMessage decodedMessageNoInterleave;

enigma_score_function_t enigmaScoreOptNoInterleave = {
    triscoreNoInterleave,
    biscoreNoInterleave,
    icscoreNoInterleave,
    uniscoreNoInterleave };

__attribute__ (( optimize( "sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops" ) ))
static inline
void DecodeScoredMessagePartNoInterleave( const const Key* const restrict key, int len, union ScoringDecodedMessage* output ){
    const PermutationMap_t* const restrict stbrett = &key->stbrett;
    int i;
    for( i = 0; i < len - 15; i += 16 ) {
        output->plain[ 0 + i] = decode(  0, i, stbrett );
        output->plain[ 1 + i] = decode(  1, i, stbrett );
        output->plain[ 2 + i] = decode(  2, i, stbrett );
        output->plain[ 3 + i] = decode(  3, i, stbrett );
        output->plain[ 4 + i] = decode(  4, i, stbrett );
        output->plain[ 5 + i] = decode(  5, i, stbrett );
        output->plain[ 6 + i] = decode(  6, i, stbrett );
        output->plain[ 7 + i] = decode(  7, i, stbrett );
        output->plain[ 8 + i] = decode(  8, i, stbrett );
        output->plain[ 9 + i] = decode(  9, i, stbrett );
        output->plain[10 + i] = decode( 10, i, stbrett );
        output->plain[11 + i] = decode( 11, i, stbrett );
        output->plain[12 + i] = decode( 12, i, stbrett );
        output->plain[13 + i] = decode( 13, i, stbrett );
        output->plain[14 + i] = decode( 14, i, stbrett );
        output->plain[15 + i] = decode( 15, i, stbrett );
    }
    for ( ; i < len - 3; i += 4 ) {
        output->plain[ 0 + i] = decode( 0, i, stbrett );
        output->plain[ 1 + i] = decode( 1, i, stbrett );
        output->plain[ 2 + i] = decode( 2, i, stbrett );
        output->plain[ 3 + i] = decode( 3, i, stbrett );
    }
    for ( ; i < len; i++ ) {
        output->plain[i] = decode( 0, i, stbrett );
    }
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
int triscoreNoInterleave( const Key* const restrict key, int len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMessageNoInterleave );
    uint8_t length = len;
    int s = 0;
    uint8_t i;
    for ( i = 0; i < length - 2; ++i ){
        s += tridict[decodedMessageNoInterleave.plain[i]][decodedMessageNoInterleave.plain[i+1]][decodedMessageNoInterleave.plain[i+2]];
    }
    return s;
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static double icscoreNoInterleave( const Key* const restrict key, int len ) {
    if (len < 2) {
        return 0;
    }
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMessageNoInterleave );

    uint8_t f[26] = {0};
    uint8_t i;
    uint8_t length = len;
    for( i = 0; i < length; ++i ){
        f[decodedMessageNoInterleave.plain[i]]++;
    }

    int S0, S1, S2, S3;
    S0 = S1 = S2 = S3 = 0;
    for( i = 0; i < 26 - 3; i += 4 ) {
        S0 += f[i]     *( f[i]     - 1 );
        S1 += f[i + 1] *( f[i + 1] - 1 );
        S2 += f[i + 2] *( f[i + 2] - 1 );
        S3 += f[i + 3] *( f[i + 3] - 1 );
    }
    S0 += f[24] * ( f[24] - 1 );
    S1 += f[25] * ( f[25] - 1 );

    return ( double )( ( S0 + S1 ) + ( S2 + S3 ) ) / ( len * ( len - 1 ) );
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int uniscoreNoInterleave( const Key* key, int len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMessageNoInterleave );
    int s = 0;
    uint8_t i;
    uint8_t length = len;
    for ( i = 0; i < length; i++) {
        s += unidict[decodedMessageNoInterleave.plain[i]];
    }
    return s;
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int biscoreNoInterleave( const Key* const restrict key, int len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMessageNoInterleave );
    uint8_t i;
    uint8_t length = len;
    int s = 0;
    for( i = 0; i < length - 1; i++ ) {
        s += bidict[decodedMessageNoInterleave.plain[i]][decodedMessageNoInterleave.plain[i + 1]];
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
