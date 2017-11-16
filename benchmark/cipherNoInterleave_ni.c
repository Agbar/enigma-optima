#include "cipherNoInterleave_ni.h"
#include "scoreNoInterleave.h"
#include "scoreNoInterleave_inlines.h"

void DecodeMessageBasicNoInterleave( const Key* const restrict key, int len ){
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
}

int  TriscoreBasicNoInterleave( scoreLength_t len ){
    return ComputeTriscoreFromDecodedMsgNoInterleave( &decodedMsgPartNoInterleave, len );
}
