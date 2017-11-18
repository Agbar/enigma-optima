#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageAvx( const Key* const restrict key, int len );
int  BiscoreAvx( scoreLength_t len );
int  IcscoreAvx( scoreLength_t len );
int  TriscoreAvx( scoreLength_t len );
