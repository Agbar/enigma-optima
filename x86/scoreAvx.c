#include <immintrin.h>

#ifndef __AVX__
# error AVX not defined
#endif

#include "..\dict.h"
#include "..\ciphertext.h"
#include "scoreAvx.h"
#include "scoreSsse3.h"
#include "cipherSsse3.h"
#include "cipherSsse3_inlines.h"
#include "../score_inlines.h"

// SSSE3 scores
static double icscoreAvx( const Key* const restrict key, scoreLength_t len );
static int   uniscoreAvx( const Key* const restrict key, scoreLength_t len );
static int    biscoreAvx( const Key* const restrict key, scoreLength_t len );
static int   triscoreAvx( const Key* const restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx = { triscoreAvx,  biscoreAvx , icscoreAvx,  uniscoreAvx } ;

union ScoringDecodedMessage decodedMsgPartAvx;

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static double icscoreAvx( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeIcscoreFromDecodedMsgSsse3( &decodedMsgPartAvx, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int uniscoreAvx( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartAvx, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int biscoreAvx( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeBiscoreFromDecodedMsg( &decodedMsgPartAvx, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int triscoreAvx( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeTriscoreFromDecodedMsg( &decodedMsgPartAvx, len );
}
