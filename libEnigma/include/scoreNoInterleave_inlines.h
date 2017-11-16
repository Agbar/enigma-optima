#pragma once
#include <stdint.h>
#include "cipher.h"
#include "dict.h"
#include "key.h"

__attribute__ (( optimize( "sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops" ) ))
static inline
void DecodeScoredMessagePartNoInterleave( const Key* const restrict key, scoreLength_t len, union ScoringDecodedMessage* output ){
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

__attribute__ (( optimize( "sched-stalled-insns=0,sched-stalled-insns-dep=16,unroll-loops" ) ))
static inline
void DecodeScoredMessagePartNoInterleaveSimple( const Key* const restrict key, scoreLength_t len, union ScoringDecodedMessage* output ){
    const PermutationMap_t* const restrict stbrett = &key->stbrett;
    int i;
    for ( i = 0 ; i < len; i++ ) {
        output->plain[i] = decode( 0, i, stbrett );
    }
}
