#pragma GCC target ("sse2")

#ifndef __SSE2__
# error SSE2 not defined
#endif

#include <immintrin.h>

#include "cipherSse2_ni.h"
#include "score_inlines.h"
#include "x86/cipherSsse3_inlines.h"

__attribute__ ((flatten))
int TriscoreSse2( scoreLength_t len ) {
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
int BiscoreSse2( scoreLength_t len ) {
    return ComputeBiscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}
