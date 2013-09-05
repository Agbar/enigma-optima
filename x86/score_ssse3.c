#pragma GCC target ("ssse3")
#include <immintrin.h>

#include "..\dict.h"
#include "..\ciphertext.h"
#include "score_ssse3.h"
#include "cipher_ssse3.h"
#include "cipher_ssse3_inlines.h"

#ifndef __SSSE3__
# error SSSE3 not defined
#endif

// SSSE3 scores
static double icscore_ssse3( const Key* const restrict key, int len );
static int   uniscore_ssse3( const Key* const restrict key, int len );
static int    biscore_ssse3( const Key* const restrict key, int len );
static int   triscore_ssse3( const Key* const restrict key, int len );

enigma_score_function_t enigma_score_ssse3  = { triscore_ssse3,  biscore_ssse3 , icscore_ssse3,  uniscore_ssse3 } ;

union ScoringDecodedMessage decodedMessageSsse3;

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
inline
static void DecodeScoredMessagePart( const const Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;

    while( messageBite < ( len + 15 ) / 16 )
    {
        /* Worst case:
         *  P0123456789ABCDE    R-ring position (turnovers on 12 & 25, coded C & P)
         *  0123456789ABCDEF    characters in bite
         *  |||           |
         *  |||           +-  turnover on second notch of R ring
         *  ||+- turnover caused by M-ring (turning L- & M- rings).
         *  |+-- turnover setting M-ring to turnover position
         *  +--- last character from previous bite
         *
         *  In the worst case there are 4 lookups per bite.
         */
        uint_least16_t lookupsToNextBite = PathLookupSsse3.nextBite[messageBite] - lookupNumber;
        v16qi cBite = {0};
        lookupNumber += lookupsToNextBite;
        switch( lookupsToNextBite ) {
        case 4:
            cBite  = enigma_cipher_decode_ssse3( messageBite, lookupNumber - 4, key );
        case 3:
            cBite |= enigma_cipher_decode_ssse3( messageBite, lookupNumber - 3, key );
        case 2:
            cBite |= enigma_cipher_decode_ssse3( messageBite, lookupNumber - 2, key );
        case 1:
            cBite |= enigma_cipher_decode_ssse3( messageBite, lookupNumber - 1, key );
            break;
        default:
            exit(5);
        }
        // store whole decoded bite
        output -> vector16[messageBite] = cBite;
        messageBite++;
    }
}

__attribute__ ((optimize("unroll-loops")))
static double icscore_ssse3( const Key* const restrict key, int len )
{
    DecodeScoredMessagePart( key, len, &decodedMessageSsse3 );

    int f[26] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[decodedMessageSsse3.plain[i]]++;
    }

    int S = 0;
    for( i = 0; i < 26; i++ ) {
        S += f[i] * ( f[i] - 1 );
    }
    return ( double )S / ( len * ( len - 1 ) );
}

__attribute__ ((optimize("unroll-loops")))
static int uniscore_ssse3( const Key* const restrict key, int len )
{
    DecodeScoredMessagePart( key, len, &decodedMessageSsse3 );

    int score = 0, i;
    for( i = 0; i < len; i++ ) {
        score += unidict[i];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
static int biscore_ssse3( const Key* const restrict key, int len )
{
    DecodeScoredMessagePart( key, len, &decodedMessageSsse3 );

    int score = 0;
    int x;
    //uint8_t length = len;
    for( x = 0; x < len - 1; ++x )
    {
        score += bidict[decodedMessageSsse3.plain[x]][decodedMessageSsse3.plain[x + 1]];
    }

    return score;
}

__attribute__ ((optimize("unroll-loops")))
static int triscore_ssse3( const Key* const restrict key, int len )
{
    DecodeScoredMessagePart( key, len, &decodedMessageSsse3 );

    int x, score = 0;
    for( x = 0; x < len - 2; ++x )
    {
        score += tridict[decodedMessageSsse3.plain[x]][decodedMessageSsse3.plain[x + 1]][decodedMessageSsse3.plain[x + 2]];
    }

    return score;
}
