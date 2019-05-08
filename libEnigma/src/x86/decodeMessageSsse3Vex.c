#pragma GCC target("avx")

#include "x86/decodeMessageSsse3Vex.h"

#include "decodeMessageSsse3_inlines.h"

__attribute((hot))
void DecodeScoredMessagePartSsse3Vex( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output ){
    return staticDecodeScoredMessagePartSsse3( key, len, output );
}
