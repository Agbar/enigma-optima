#include "dict.h"
#include "key.h"
#include "score.h"
#include "scoreNoInterleave.h"
#include "scoreNoInterleave_inlines.h"

// default scores
static int     uniscoreNoInterleave( const Key* const restrict key, scoreLength_t len );
static int      biscoreNoInterleave( const Key* const restrict key, scoreLength_t len );
static int     triscoreNoInterleave( const Key* const restrict key, scoreLength_t len );
static uint16_t icscoreNoInterleave( const Key* const restrict key, scoreLength_t len );

union ScoringDecodedMessage decodedMsgPartNoInterleave;

enigma_score_function_t enigmaScoreOptNoInterleave = {
    triscoreNoInterleave,
    biscoreNoInterleave,
    icscoreNoInterleave,
    uniscoreNoInterleave };

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
int triscoreNoInterleave( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    return ComputeTriscoreFromDecodedMsgNoInterleave( &decodedMsgPartNoInterleave, len );
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static uint16_t icscoreNoInterleave( const Key* const restrict key, scoreLength_t len ) {
    if (len < 2) {
        return 0;
    }
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );

    uint8_t f[26] = {0};
    uint8_t i;
    uint8_t length = len;
    for( i = 0; i < length; ++i ){
        f[decodedMsgPartNoInterleave.plain[i]]++;
    }

    uint16_t S0, S1, S2, S3;
    S0 = S1 = S2 = S3 = 0;
    for( i = 0; i < 26 - 3; i += 4 ) {
        S0 += f[i]     *( f[i]     - 1 );
        S1 += f[i + 1] *( f[i + 1] - 1 );
        S2 += f[i + 2] *( f[i + 2] - 1 );
        S3 += f[i + 3] *( f[i + 3] - 1 );
    }
    S0 += f[24] * ( f[24] - 1 );
    S1 += f[25] * ( f[25] - 1 );

    return ( S0 + S1 ) + ( S2 + S3 );
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int uniscoreNoInterleave( const Key* key, scoreLength_t len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    int s = 0;
    uint8_t i;
    uint8_t length = len;
    for ( i = 0; i < length; i++) {
        s += unidict[decodedMsgPartNoInterleave.plain[i]];
    }
    return s;
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int biscoreNoInterleave( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    return ComputeBiscoreFromDecodedMsgNoInterleave( &decodedMsgPartNoInterleave, len );
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
