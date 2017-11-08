#pragma GCC target ("avx2")

#ifndef __AVX2__
# error AVX2 not defined
#endif

#include <immintrin.h>

#include "cipherAvx2_ni.h"
#include "score_inlines.h"
#include "x86/cipherAvx2_inlines.h"
#include "x86/scoreAvx2.h"

__attribute__ ((flatten))
void DecodeMessageAvx2( const Key* const restrict key, int len ) {
    DecodeScoredMessagePartAvx2( key, len, &decodedMsgPartAvx2 );
}

__attribute__ ((flatten))
int TriscoreAvx2( scoreLength_t len ) {
    return ComputeTriscoreFromDecodedMsg( &decodedMsgPartAvx2, len );
}
