#pragma GCC target ("avx")

#ifndef __AVX__
# error AVX not defined
#endif

#include <immintrin.h>

#include "cipherAvx_ni.h"
#include "score_inlines.h"
#include "x86/cipherSsse3_inlines.h"
#include "x86/scoreAvx.h"
