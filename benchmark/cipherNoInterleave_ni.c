#include "cipherNoInterleave_ni.h"
#include "score_inlines.h"
#include "scoreNoInterleave.h"
#include "scoreNoInterleave_inlines.h"

void DecodeMessageBasicNoInterleave( const Key* const restrict key, int len ){
    DecodeScoredMessagePartNoInterleave( key, len, &decodedMsgPartNoInterleave );
}

int  BiscoreSimple( scoreLength_t len ){
    return ComputeBiscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}

uint16_t IcscoreSimple( scoreLength_t len ){
    return ComputeIcscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}

int  TriscoreBasicNoInterleave( scoreLength_t len ){
    return ComputeTriscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}

int  UniscoreBasicNoInterleave( scoreLength_t len ){
    return ComputeUniscoreFromDecodedMsg( &decodedMsgPartNoInterleave, len );
}
