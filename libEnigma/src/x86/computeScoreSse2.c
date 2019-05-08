#pragma GCC target ("sse2")

#include "x86/computeScoreSse2.h"

#include "computeScoreSse2_inlines.h"

int ComputeTriscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeTriscoreFromDecodedMsgSse2( msg, len );
}

int ComputeBiscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeBiscoreFromDecodedMsgSse2( msg, len );
}