#include <immintrin.h>

#ifndef __SSSE3__
# error SSSE3 not defined
#endif

#include "../dict.h"
#include "../ciphertext.h"
#include "scoreSsse3.h"
#include "cipherSsse3.h"
#include "cipherSsse3_inlines.h"
#include "../score_inlines.h"

// SSSE3 scores
static double icscoreSsse3( const Key* const restrict key, scoreLength_t len );
static int   uniscoreSsse3( const Key* const restrict key, scoreLength_t len );
static int    biscoreSsse3( const Key* const restrict key, scoreLength_t len );
static int   triscoreSsse3( const Key* const restrict key, scoreLength_t len );

inline extern
int     ComputeTriscoreFromDecodedMsgSse2( union ScoringDecodedMessage* msg, scoreLength_t len );
inline extern
void    Unpack_v16qi( v16qi in, v8hi* lo, v8hi *hi );
inline extern
v16qi   MOVDQU( v16qi* p );

enigma_score_function_t enigmaScoreSsse3 = { triscoreSsse3,  biscoreSsse3 , icscoreSsse3,  uniscoreSsse3 } ;

union ScoringDecodedMessage decodedMsgPartSsse3;

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static double icscoreSsse3( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeIcscoreFromDecodedMsgSsse3( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int uniscoreSsse3( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int biscoreSsse3( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeBiscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int triscoreSsse3( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}
