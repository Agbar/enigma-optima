#pragma once

#include "dict.h"
#include "score.h"

__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeTriscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ){
    int x, score = 0;
    for( x = 0; x < len - 2; ++x ) {
        score += tridict[ echar_0_based_index( msg->plain[x] ) ]
                        [ echar_0_based_index( msg->plain[x + 1] ) ]
                        [ echar_0_based_index( msg->plain[x + 2] ) ];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeBiscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ) {
    int score = 0, x;
    for( x = 0; x < len - 1; ++x ) {
        score += bidict[ echar_0_based_index( msg->plain[x] ) ]
                       [ echar_0_based_index( msg->plain[x + 1] ) ];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeUniscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0, i;
    for( i = 0; i < len; i++ ) {
        score += unidict[ echar_0_based_index( msg->plain[i] ) ];
    }
    return score;
}

#include "config/types.h"
__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
uint16_t ComputeIcscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ){
    uint8_t f[32] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[ echar_0_based_index( msg->plain[i] ) ]++;
    }

    uint16_t sum = 0;
    for( i = 0; i < 26; i++ ) {
        sum += f[i] * ( f[i] - 1 );
    }

    return sum;
}
