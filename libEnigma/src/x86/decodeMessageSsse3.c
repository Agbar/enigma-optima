#pragma GCC target("ssse3")

#include "x86/decodeMessageSsse3.h"

#include "decodeMessageSsse3_inlines.h"

__attribute((hot))
void DecodeScoredMessagePartSsse3( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output ){
    return staticDecodeScoredMessagePartSsse3( key, len, output );
}
