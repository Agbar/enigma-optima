#ifndef SCORE_INLINES_HEADER_INCLUDED
#define SCORE_INLINES_HEADER_INCLUDED

#include "score.h"

__attribute__ ((optimize("unroll-loops")))
inline
int ComputeTriscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ){
    int x, score = 0;
    for( x = 0; x < len - 2; ++x ) {
        score += tridict[msg->plain[x]][msg->plain[x + 1]][msg->plain[x + 2]];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
inline
int ComputeBiscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ) {
    int score = 0, x;
    for( x = 0; x < len - 1; ++x ) {
        score += bidict[msg->plain[x]][msg->plain[x + 1]];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
inline
int ComputeUniscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0, i;
    for( i = 0; i < len; i++ ) {
        score += unidict[msg->plain[i]];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
inline
double ComputeIcscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len ){
    uint16_t f[26] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[msg->plain[i]]++;
    }

    STATIC_ASSERT ( UINT16_MAX > CT * CT, "uint16_t is to narrow for current CT value. Use ie. uint32_t." );
    uint16_t S = 0;
    for( i = 0; i < 26; i++ ) {
        S += f[i] * ( f[i] - 1 );
    }
    return ( double )S / ( len * ( len - 1 ) );
}

#endif
