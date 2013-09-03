#pragma GCC target ("ssse3")
#include <immintrin.h>

#include "..\dict.h"
#include "..\ciphertext.h"
#include "score_ssse3.h"
#include "cipher_ssse3.h"
#include "cipher_ssse3_inlines.h"

// SSSE3 scores
static double icscore_ssse3( const Key* const restrict key, int len );
static int   uniscore_ssse3( const Key* const restrict key, int len );
static int    biscore_ssse3( const Key* const restrict key, int len );
static int   triscore_ssse3( const Key* const restrict key, int len );

enigma_score_function_t enigma_score_ssse3  = { triscore_ssse3,  biscore_ssse3 , icscore_ssse3,  uniscore_ssse3 } ;

union ScoringDecodedMessage decodedMessageSsse3;

__attribute__ ((optimize("unroll-loops")))
inline
static void DecodeScoredMessagePart( const const Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    int messageBite  = 0;
    int lookupNumber = 0;
    int nextBiteCounter = 0;

    v16qi cBite = {0};
    while( messageBite < ( len + 15 ) / 16 )
    {
        v16qi cBitePart = enigma_cipher_decode_ssse3( messageBite, lookupNumber, key );

        cBite |= cBitePart;

        ++lookupNumber;
        if( lookupNumber > PathLookupSsse3.nextBite[nextBiteCounter] ) {
            // store whole decoded bite
            output -> vector16[messageBite] = cBite;
            cBite ^= cBite;
            nextBiteCounter++;
            messageBite++;
        }
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
