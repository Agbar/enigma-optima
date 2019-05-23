#pragma once

#include "score.h"

uint32_t ComputeTriscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len );

int ComputeBiscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len );