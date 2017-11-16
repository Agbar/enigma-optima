#include "cipherNoInterleave_ni.h"
#include "score_inlines.h"
#include "scoreNoInterleave.h"
#include "scoreNoInterleave_inlines.h"

void DecodeMessageBasicNoInterleave( const Key* const restrict key, int len ){
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
}

int  BiscoreBasicNoInterleave( scoreLength_t len ){
    return ComputeBiscoreFromDecodedMsgNoInterleave( &decodedMsgPartNoInterleave, len );
}

int  TriscoreBasicNoInterleave( scoreLength_t len ){
    return ComputeTriscoreFromDecodedMsgNoInterleave( &decodedMsgPartNoInterleave, len );
}

int  UniscoreBasicNoInterleave( scoreLength_t len ){
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}
