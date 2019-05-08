#pragma GCC target ("avx")

#include "x86/computeScoreSsse3Vex.h"

#include "computeScoreSsse3_inlines.h"

uint16_t ComputeIcscoreFromDecodedMsgSsse3Vex( union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeIcscoreFromDecodedMsgSsse3( msg, len );
}
