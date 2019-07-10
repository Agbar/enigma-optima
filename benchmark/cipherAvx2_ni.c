#pragma GCC target ("avx2")


#include "cipherAvx2_ni.h"
#include "x86/computeScoreAvx2.h"
#include "x86/decodeMessageAvx2.h"
#include "x86/scoreAvx2.h"

__attribute__ ((flatten))
void DecodeMessageAvx2( const struct Key* const restrict key, int len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
}

__attribute__ ((flatten))
int  IcscoreAvx2( scoreLength_t len ){
    return ComputeIcscoreFromDecodedMsgAvx2( &decodedMsgPartAvx2, len );
}

__attribute__ ((flatten))
uint32_t TriscoreAvx2( scoreLength_t len ){
    return ComputeTriscoreFromDecodedMsgAvx2( &decodedMsgPartAvx2, len );
}
