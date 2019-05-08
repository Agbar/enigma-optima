#pragma once

#include "score.h"

int ComputeTriscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len );

int ComputeBiscoreFromDecodedMsgSse2Vex( const union ScoringDecodedMessage* msg, scoreLength_t len );
