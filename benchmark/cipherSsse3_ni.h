#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageSsse3( const Key* const restrict key, int len );
int  IcscoreSsse3( scoreLength_t len );
