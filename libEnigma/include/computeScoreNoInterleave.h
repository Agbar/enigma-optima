#pragma once

#include <stdint.h>

#include "score.h"

int ComputeTriscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len );
int ComputeBiscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len );
int ComputeUniscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len );
uint16_t ComputeIcscoreFromDecodedMsg( union ScoringDecodedMessage* msg, scoreLength_t len );