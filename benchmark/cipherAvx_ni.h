#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageAvx( const struct Key* restrict key, int len );
int  IcscoreAvx( scoreLength_t len );
int  BiscoreAvx( scoreLength_t len );
int  TriscoreAvx( scoreLength_t len );
