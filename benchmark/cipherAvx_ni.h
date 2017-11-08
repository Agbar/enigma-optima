#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageAvx( const Key* const restrict key, int len );
int  TriscoreAvx( scoreLength_t len );
