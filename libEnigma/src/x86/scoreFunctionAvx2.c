#pragma GCC target ("avx2")

#include <immintrin.h>

#ifndef __AVX2__
# error AVX2 not defined
#endif

#include "dict.h"
#include "ciphertext.h"
#include "x86/cipherAvx2.h"
#include "x86/computeScoreSse2Vex.h"
#include "x86/computeScoreAvx2.h"
#include "x86/decodeMessageAvx2.h"
#include "x86/scoreAvx2.h"
#include "score_inlines.h"

// AVX2 scores
static uint16_t icscoreAvx2( const struct Key* restrict key, scoreLength_t len );
static int     uniscoreAvx2( const struct Key* restrict key, scoreLength_t len );
static int      biscoreAvx2( const struct Key* restrict key, scoreLength_t len );
static int     triscoreAvx2( const struct Key* restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx2 = { triscoreAvx2,  biscoreAvx2 , icscoreAvx2,  uniscoreAvx2 } ;

union ScoringDecodedMessage decodedMsgPartAvx2;

static uint16_t icscoreAvx2( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeIcscoreFromDecodedMsgAvx2( &decodedMsgPartAvx2, len );
}

static int uniscoreAvx2( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartAvx2, len );
}

static int biscoreAvx2( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeBiscoreFromDecodedMsg( &decodedMsgPartAvx2, len );
}

static int triscoreAvx2( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeTriscoreFromDecodedMsgAvx2( &decodedMsgPartAvx2, len );
}
