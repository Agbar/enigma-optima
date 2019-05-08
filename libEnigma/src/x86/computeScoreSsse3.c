#pragma GCC target ("ssse3")

#include "x86/computeScoreSsse3.h"

#include "computeScoreSsse3_inlines.h"

uint16_t ComputeIcscoreFromDecodedMsgSsse3( union ScoringDecodedMessage* msg, scoreLength_t len ) {
    return staticComputeIcscoreFromDecodedMsgSsse3( msg, len );
}
