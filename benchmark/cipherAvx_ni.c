#pragma GCC target ("avx")

#ifndef __AVX__
# error AVX not defined
#endif

#include <immintrin.h>

#include "cipherAvx_ni.h"
#include "x86/cipherSsse3_inlines.h"
#include "x86/scoreAvx.h"

__attribute__ ((flatten))
void DecodeMessageAvx( const Key* const restrict key, int len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
}

__attribute__ ((flatten))
int TriscoreAvx( scoreLength_t len ) {
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartAvx, len );
}
