#include "cipherSse2_ni.h"
#include "x86/cipherSsse3.h"
#include "x86/computeScoreSse2.h"

__attribute__ ((flatten))
int TriscoreSse2( scoreLength_t len ) {
    return ComputeTriscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}

__attribute__ ((flatten))
int BiscoreSse2( scoreLength_t len ) {
    return ComputeBiscoreFromDecodedMsgSse2( &decodedMsgPartSsse3, len );
}
