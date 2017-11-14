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
int TriscoreSse3( scoreLength_t len ) {
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
int BiscoreSsse3( scoreLength_t len ) {
    return ComputeBiscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
int  UniscoreSsse3( scoreLength_t len ){
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartSsse3, len );
}
