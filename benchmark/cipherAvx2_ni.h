#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageAvx2( const Key* const restrict key, int len );
int  IcscoreAvx2( scoreLength_t len );
