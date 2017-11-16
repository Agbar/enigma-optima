#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageBasicNoInterleave( const Key* const restrict key, int len );
int  TriscoreBasicNoInterleave( scoreLength_t len );
