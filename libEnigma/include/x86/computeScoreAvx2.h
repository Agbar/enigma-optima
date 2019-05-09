#pragma once

#include <stdint.h>

#include "score.h"

uint16_t ComputeIcscoreFromDecodedMsgAvx2( union ScoringDecodedMessage* msg, scoreLength_t len );

uint32_t ComputeTriscoreFromDecodedMsgAvx2( const union ScoringDecodedMessage* msg, scoreLength_t len );
