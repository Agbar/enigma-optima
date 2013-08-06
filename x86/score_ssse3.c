#pragma GCC target ("ssse3")
#include <immintrin.h>

#include "..\dict.h"
#include "..\ciphertext.h"
#include "score_ssse3.h"

// SSSE3 scores
static double icscore_ssse3(const Key* key, int len);
static int   uniscore_ssse3(const Key* key, int len);
static int    biscore_ssse3(const Key* key, int len);
static int   triscore_ssse3(const Key* key, int len);

enigma_score_function_t enigma_score_ssse3  = { triscore_ssse3,  biscore_ssse3 , icscore_ssse3,  uniscore_ssse3 } ;

static double icscore_ssse3(const Key* const restrict key, int len)
{
    // TODO: This is only a stub.
    return .0;
}

static int uniscore_ssse3(const Key* const restrict key, int len)
{
    // TODO: This is only a stub.
    return 0;
}

static int biscore_ssse3(const Key* const restrict key, int len)
{
    // TODO: This is only a stub.
    return 0;
}

static int triscore_ssse3(const Key* const restrict key, int len)
{
    // TODO: This is only a stub.
    return 0;
}
