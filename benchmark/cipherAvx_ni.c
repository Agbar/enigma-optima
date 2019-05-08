#pragma GCC target ("avx")

#ifndef __AVX__
# error AVX not defined
#endif

#include <immintrin.h>

#include "cipherAvx_ni.h"
#include "x86/computeScoreSse2Vex.h"
#include "x86/computeScoreSsse3Vex.h"
#include "x86/cipherSsse3_inlines.h"
#include "x86/scoreAvx.h"

__attribute__ ((flatten))
void DecodeMessageAvx( const struct Key* const restrict key, int len ) {
    DecodeScoredMessagePartSsse3( key, len, &decodedMsgPartAvx );
}

__attribute__ ((flatten))
int  IcscoreAvx( scoreLength_t len ){
    return ComputeIcscoreFromDecodedMsgSsse3Vex( &decodedMsgPartAvx, len );
}

__attribute__ ((flatten))
int  BiscoreAvx( scoreLength_t len ){
    return ComputeBiscoreFromDecodedMsgSse2Vex( &decodedMsgPartAvx, len );
}

__attribute__ ((flatten))
int  TriscoreAvx( scoreLength_t len ){
    return ComputeTriscoreFromDecodedMsgSse2Vex( &decodedMsgPartAvx, len );
}
