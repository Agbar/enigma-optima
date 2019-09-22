#include "computeScoreNoInterleave.h"
#include "dict.h"
#include "key.h"
#include "score.h"
#include "score_inlines.h"
#include "scoreNoInterleave.h"
#include "scoreNoInterleave_inlines.h"

// default scores
static int     uniscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len );
static int      biscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len );
static int     triscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len );
static int      icscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len );

union ScoringDecodedMessage decodedMsgPartNoInterleave;

enigma_score_function_t enigmaScoreOptNoInterleave = {
    triscoreNoInterleave,
    biscoreNoInterleave,
    icscoreNoInterleave,
    uniscoreNoInterleave };

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
int triscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    return ComputeTriscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int icscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len ) {
    if (len < 2) {
        return 0;
    }
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    return ComputeIcscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int uniscoreNoInterleave( const struct Key* const key, scoreLength_t len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}

__attribute__ ((optimize("sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops")))
static int biscoreNoInterleave( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
    return ComputeBiscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
