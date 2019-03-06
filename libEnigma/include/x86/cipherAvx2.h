#ifndef CIPHERTEXT_AVX2_H_INCLUDED
#define CIPHERTEXT_AVX2_H_INCLUDED

#include "cipher.h"
#include "score.h"

struct LookupChunkAvx2_t {
        union PermutationMap_t mapping;
        v32qu mask;
};

struct PathLookupAvx2_t
{
    struct LookupChunkAvx2_t lookups[24];
    uint_least16_t nextBite[24];        ///< First lookup number in next bite.
    // mapping forward and reverse
    union PermutationMap_t r_ring[2];
    union v32_echar_delta firstRRingOffset;
};

extern struct PathLookupAvx2_t PathLookupAvx2;

extern enigma_cipher_function_t enigma_cipher_DecoderLookupAvx2;

#endif
