#pragma GCC target ("avx")

#include <immintrin.h>

#ifndef __AVX__
# error AVX not defined
#endif

#include "dict.h"
#include "ciphertext.h"
#include "x86/scoreAvx.h"
#include "x86/scoreSsse3.h"
#include "x86/cipherSsse3.h"
#include "x86/cipherSsse3_inlines.h"
#include "score_inlines.h"

// SSSE3 scores
static uint16_t icscoreAvx( const struct Key* restrict key, scoreLength_t len );
static int     uniscoreAvx( const struct Key* restrict key, scoreLength_t len );
static int      biscoreAvx( const struct Key* restrict key, scoreLength_t len );
static int     triscoreAvx( const struct Key* restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx = { triscoreAvx,  biscoreAvx , icscoreAvx,  uniscoreAvx } ;

union ScoringDecodedMessage decodedMsgPartAvx;

static uint16_t icscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeIcscoreFromDecodedMsgSsse3( &decodedMsgPartAvx, len );
}

static int uniscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartAvx, len );
}

static int biscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeBiscoreFromDecodedMsgSse2( &decodedMsgPartAvx, len );
}

static int triscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartAvx, len );
}
