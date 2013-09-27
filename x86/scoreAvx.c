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

// SSSE3 scores
static double icscoreAvx( const Key* const restrict key, scoreLength_t len );
static int   uniscoreAvx( const Key* const restrict key, scoreLength_t len );
static int    biscoreAvx( const Key* const restrict key, scoreLength_t len );
static int   triscoreAvx( const Key* const restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx = { triscoreAvx,  biscoreAvx , icscoreAvx,  uniscoreAvx } ;

union ScoringDecodedMessage decodedMsgPartAvx;

__attribute__ ((optimize("unroll-loops")))
static double icscoreAvx( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );

    uint16_t f[26] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[decodedMsgPartAvx.plain[i]]++;
    }

    STATIC_ASSERT ( UINT16_MAX > CT * CT, "uint16_t is to narrow for current CT value. Use ie. uint32_t." );
    uint16_t S = 0;
    for( i = 0; i < 26; i++ ) {
        S += f[i] * ( f[i] - 1 );
    }
    return ( double )S / ( len * ( len - 1 ) );
}

__attribute__ ((optimize("unroll-loops")))
static int uniscoreAvx( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );

    int score = 0, i;
    for( i = 0; i < len; i++ ) {
        score += unidict[i];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
static int biscoreAvx( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );

    int score = 0;
    int x;
    //uint8_t length = len;
    for( x = 0; x < len - 1; ++x )
    {
        score += bidict[decodedMsgPartAvx.plain[x]][decodedMsgPartAvx.plain[x + 1]];
    }

    return score;
}

__attribute__ ((optimize("unroll-loops")))
static int triscoreAvx( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );

    int x, score = 0;
    for( x = 0; x < len - 2; ++x )
    {
        score += tridict[decodedMsgPartAvx.plain[x]][decodedMsgPartAvx.plain[x + 1]][decodedMsgPartAvx.plain[x + 2]];
    }

    return score;
}
