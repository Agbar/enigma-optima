#pragma once

#include <stdint.h>

#include "score.h"

uint32_t ComputeTriscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len );
int ComputeBiscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len );
int ComputeUniscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len );
uint16_t ComputeIcscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len );