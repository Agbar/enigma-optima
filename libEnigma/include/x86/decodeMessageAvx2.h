#pragma once

#include "key.h"
#include "score.h"

void DecodeScoredMessagePartAvx2( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output );
