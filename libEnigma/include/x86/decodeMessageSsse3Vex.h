#pragma once

#include "key.h"
#include "score.h"

void DecodeScoredMessagePartSsse3Vex( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output );
