#pragma GCC target ("avx")

#include <immintrin.h>

#include "dict.h"
#include "ciphertext.h"
#include "x86/computeScoreSse2Vex.h"
#include "x86/computeScoreSsse3Vex.h"
#include "x86/decodeMessageSsse3Vex.h"
#include "x86/scoreAvx.h"
#include "x86/scoreSsse3.h"
#include "x86/cipherSsse3.h"
#include "computeScoreNoInterleave.h"

// AVX scores
static int      icscoreAvx( const struct Key* restrict key, scoreLength_t len );
static int     uniscoreAvx( const struct Key* restrict key, scoreLength_t len );
static int      biscoreAvx( const struct Key* restrict key, scoreLength_t len );
static int     triscoreAvx( const struct Key* restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx = { triscoreAvx,  biscoreAvx , icscoreAvx,  uniscoreAvx } ;

union ScoringDecodedMessage decodedMsgPartAvx;

static int icscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3Vex( key, len, &decodedMsgPartAvx );
    return ComputeIcscoreFromDecodedMsgSsse3Vex( &decodedMsgPartAvx, len );
}

static int uniscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3Vex( key, len, &decodedMsgPartAvx );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartAvx, len );
}

static int biscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3Vex( key, len, &decodedMsgPartAvx );
    return ComputeBiscoreFromDecodedMsgSse2Vex( &decodedMsgPartAvx, len );
}

static int triscoreAvx( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3Vex( key, len, &decodedMsgPartAvx );
    return ComputeTriscoreFromDecodedMsgSse2Vex( &decodedMsgPartAvx, len );
}
