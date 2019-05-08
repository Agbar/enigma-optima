#pragma once

#include <stdint.h>

#include "score.h"

uint16_t ComputeIcscoreFromDecodedMsgSsse3Vex( union ScoringDecodedMessage* msg, scoreLength_t len );
