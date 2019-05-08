#pragma once

#include "key.h"
#include "score.h"

void DecodeScoredMessagePartSsse3( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output );
