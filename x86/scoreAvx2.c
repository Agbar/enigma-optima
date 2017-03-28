#include <immintrin.h>

#ifndef __AVX2__
# error AVX2 not defined
#endif

#include "dict.h"
#include "../ciphertext.h"
#include "scoreAvx2.h"
#include "cipherAvx2.h"
#include "cipherAvx2_inlines.h"
#include "../score_inlines.h"
#include "cipherSsse3_inlines.h"

// SSSE3 scores
static double icscoreAvx2( const Key* const restrict key, scoreLength_t len );
static int   uniscoreAvx2( const Key* const restrict key, scoreLength_t len );
static int    biscoreAvx2( const Key* const restrict key, scoreLength_t len );
static int   triscoreAvx2( const Key* const restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreAvx2 = { triscoreAvx2,  biscoreAvx2 , icscoreAvx2,  uniscoreAvx2 } ;

union ScoringDecodedMessage decodedMsgPartAvx2;

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
inline
static void DecodeScoredMessagePartAvx2( const Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;
    v32qi currentRRingOffset = PathLookupAvx2.firstRRingOffset;
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
        v32qi currentBite = ciphertext.vector32[messageBite];
        v32qi predecoded = DecodeBiteForwardCommonAvx2( currentBite, currentRRingOffset, key );

        switch( lookupsToNextBite ) {
        case 5:
            cBite  = DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 5 );
        case 4:
            cBite |= DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 4 );
        case 3:
            cBite |= DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 3 );
        case 2:
            cBite |= DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 2 );
        case 1:
            cBite |= DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 1 );
            break;
        default:
            exit_d(5);
            UNREACHABLE();
        }
        cBite = DecodeBiteBackwardCommonAvx2( cBite, currentRRingOffset, key );
        // store whole decoded bite
        output -> vector32[messageBite] = cBite;
        messageBite++;
        currentRRingOffset = AddMod26_v32qi_int8( currentRRingOffset, 32 % 26 );
    }
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static double icscoreAvx2( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeIcscoreFromDecodedMsgAvx2( &decodedMsgPartAvx2, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int uniscoreAvx2( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartAvx2, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int biscoreAvx2( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeBiscoreFromDecodedMsg( &decodedMsgPartAvx2, len );
}

__attribute__ ((flatten))
__attribute__ ((optimize("unroll-loops")))
static int triscoreAvx2( const Key* const restrict key, scoreLength_t len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
    return ComputeTriscoreFromDecodedMsg( &decodedMsgPartAvx2, len );
}
