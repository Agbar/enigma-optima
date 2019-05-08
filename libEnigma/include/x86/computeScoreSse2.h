#pragma once

#include "score.h"

int ComputeTriscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len );

int ComputeBiscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len );