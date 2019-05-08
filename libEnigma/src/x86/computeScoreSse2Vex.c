#pragma GCC target ("avx")

#include "x86/computeScoreSse2Vex.h"

#include "computeScoreSse2_inlines.h"

__attribute__ ((hot))
int ComputeTriscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeTriscoreFromDecodedMsgSse2( msg, len );
}

__attribute__ ((hot))
int ComputeBiscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeBiscoreFromDecodedMsgSse2( msg, len );
}
