#pragma GCC target ("ssse3")

#include <tmmintrin.h>

#include "dict.h"
#include "ciphertext.h"
#include "x86/computeScoreSse2.h"
#include "x86/computeScoreSsse3.h"
#include "x86/decodeMessageSsse3.h"
#include "x86/scoreSsse3.h"
#include "x86/cipherSsse3.h"
#include "computeScoreNoInterleave.h"

// SSSE3 scores
static int      icscoreSsse3( const struct Key* restrict key, scoreLength_t len );
static int     uniscoreSsse3( const struct Key* restrict key, scoreLength_t len );
static int      biscoreSsse3( const struct Key* restrict key, scoreLength_t len );
static int     triscoreSsse3( const struct Key* restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreSsse3 = { triscoreSsse3,  biscoreSsse3 , icscoreSsse3,  uniscoreSsse3 } ;

union ScoringDecodedMessage decodedMsgPartSsse3;

static int icscoreSsse3( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeIcscoreFromDecodedMsgSsse3( &decodedMsgPartSsse3, len );
}

static int uniscoreSsse3( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartSsse3, len );
}

static int biscoreSsse3( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeBiscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}

static int triscoreSsse3( const struct Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}
