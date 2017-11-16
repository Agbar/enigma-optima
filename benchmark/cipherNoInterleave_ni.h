#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageBasicNoInterleave( const Key* const restrict key, int len );
int  BiscoreBasicNoInterleave( scoreLength_t len );
int  TriscoreBasicNoInterleave( scoreLength_t len );
int  UniscoreBasicNoInterleave( scoreLength_t len );
