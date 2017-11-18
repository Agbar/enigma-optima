#pragma GCC target ("ssse3")

#ifndef __SSSE3__
# error SSSE3 not defined
#endif


#include <immintrin.h>

#include "cipherSsse3_ni.h"
#include "score_inlines.h"
#include "x86/cipherSsse3_inlines.h"

__attribute__ ((flatten))
void DecodeMessageSsse3( const Key* const restrict key, int len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartSsse3 );
}

__attribute__ ((flatten))
int  IcscoreSsse3( scoreLength_t len ){
    return ComputeIcscoreFromDecodedMsgSsse3( &decodedMsgPartSsse3, len );
}
