#pragma once

#include "key.h"
#include "score.h"

void DecodeMessageBasicNoInterleave( const Key* const restrict key, int len );
int  BiscoreSimple( scoreLength_t len );
int  TriscoreBasicNoInterleave( scoreLength_t len );
int  UniscoreBasicNoInterleave( scoreLength_t len );

uint16_t IcscoreSimple( scoreLength_t len );
