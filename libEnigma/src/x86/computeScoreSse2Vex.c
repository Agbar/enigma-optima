#pragma GCC target ("avx")

#include "x86/computeScoreSse2Vex.h"

#include "computeScoreSse2_inlines.h"

int ComputeTriscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeTriscoreFromDecodedMsgSse2( msg, len );
}

int ComputeBiscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeBiscoreFromDecodedMsgSse2( msg, len );
}
