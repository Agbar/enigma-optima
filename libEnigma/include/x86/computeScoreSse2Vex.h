#pragma once

#include <stdint.h>

#include "score.h"

uint32_t ComputeTriscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len );

int ComputeBiscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len );
