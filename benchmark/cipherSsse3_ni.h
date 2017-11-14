#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageSsse3( const Key* const restrict key, int len );
int  BiscoreSsse3( scoreLength_t len );
int  TriscoreSse3( scoreLength_t len );
