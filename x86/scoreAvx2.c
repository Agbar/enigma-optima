#include <immintrin.h>

#ifndef __AVX2__
# error AVX2 not defined
#endif

#include "..\dict.h"
#include "..\ciphertext.h"
#include "scoreAvx2.h"
#include "cipherAvx2.h"
#include "cipherAvx2_inlines.h"

// SSSE3 scores
static double icscoreAvx2( const Key* const restrict key, scoreLength_t len );
static int   uniscoreAvx2( const Key* const restrict key, scoreLength_t len );
static int    biscoreAvx2( const Key* const restrict key, scoreLength_t len );
static int   triscoreAvx2( const Key* const restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx2 = { triscoreAvx2,  biscoreAvx2 , icscoreAvx2,  uniscoreAvx2 } ;

union ScoringDecodedMessage decodedMsgPartAvx2;

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
inline
static void DecodeScoredMessagePart( const const Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;

    while( messageBite < ( len + 31 ) / 32 )
    {
        /* Worst case:
         *  P0123456789ABCDEFGHIJKLMNOP01234    R-ring position (turnovers on 12 & 25, coded C & P)
         *  0123456789ABCDEF0123456789ABCDEF    characters in bite
         *  |||           |            |
         *  |||           |            +
         *  |||           +-  turnover on second notch of R ring
         *  ||+- turnover caused by M-ring (turning L- & M- rings).
         *  |+-- turnover setting M-ring to turnover position
         *  +--- last character from previous bite
         *
         *  In the worst case there are 5 lookups per bite.
         */
        uint_least16_t lookupsToNextBite = PathLookupAvx2.nextBite[messageBite] - lookupNumber;
        v32qi cBite = {0};
        lookupNumber += lookupsToNextBite;
        switch( lookupsToNextBite ) {
        case 5:
            cBite  = DecodeBiteAvx2( messageBite, lookupNumber - 5, key );
        case 4:
            cBite |= DecodeBiteAvx2( messageBite, lookupNumber - 4, key );
        case 3:
            cBite |= DecodeBiteAvx2( messageBite, lookupNumber - 3, key );
        case 2:
            cBite |= DecodeBiteAvx2( messageBite, lookupNumber - 2, key );
        case 1:
            cBite |= DecodeBiteAvx2( messageBite, lookupNumber - 1, key );
            break;
        default:
            exit(5);
        }
        // store whole decoded bite
        output -> vector32[messageBite] = cBite;
        messageBite++;
    }
}

__attribute__ ((optimize("unroll-loops")))
static double icscoreAvx2( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePart( key, len, &decodedMsgPartAvx2 );

    uint16_t f[26] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[decodedMsgPartAvx2.plain[i]]++;
    }

    STATIC_ASSERT ( UINT16_MAX > CT * CT, "uint16_t is to narrow for current CT value. Use ie. uint32_t." );
    uint16_t S = 0;
    for( i = 0; i < 26; i++ ) {
        S += f[i] * ( f[i] - 1 );
    }
    return ( double )S / ( len * ( len - 1 ) );
}

__attribute__ ((optimize("unroll-loops")))
static int uniscoreAvx2( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePart( key, len, &decodedMsgPartAvx2 );

    int score = 0, i;
    for( i = 0; i < len; i++ ) {
        score += unidict[i];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
static int biscoreAvx2( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePart( key, len, &decodedMsgPartAvx2 );

    int score = 0;
    int x;
    //uint8_t length = len;
    for( x = 0; x < len - 1; ++x )
    {
        score += bidict[decodedMsgPartAvx2.plain[x]][decodedMsgPartAvx2.plain[x + 1]];
    }

    return score;
}

__attribute__ ((optimize("unroll-loops")))
static int triscoreAvx2( const Key* const restrict key, scoreLength_t len )
{
    DecodeScoredMessagePart( key, len, &decodedMsgPartAvx2 );

    int x, score = 0;
    for( x = 0; x < len - 2; ++x )
    {
        score += tridict[decodedMsgPartAvx2.plain[x]][decodedMsgPartAvx2.plain[x + 1]][decodedMsgPartAvx2.plain[x + 2]];
    }

    return score;
}
