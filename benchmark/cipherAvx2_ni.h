#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageAvx2( const Key* const restrict key, int len );
int  BiscoreAvx2( scoreLength_t len );
int  IcscoreAvx2( scoreLength_t len );
int  TriscoreAvx2( scoreLength_t len );
int  UniscoreAvx2( scoreLength_t len );

