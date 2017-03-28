#ifndef CIPHERTEXT_AVX2_H_INCLUDED
#define CIPHERTEXT_AVX2_H_INCLUDED

#include "cipher.h"
#include "score.h"

struct LookupChunkAvx2_t {
        PermutationMap_t mapping;
        v32qi mask;
};

struct PathLookupAvx2_t
{
    struct LookupChunkAvx2_t lookups[24];
    uint_least16_t nextBite[24];        ///< First lookup number in next bite.
    // mapping forward and reverse
    PermutationMap_t r_ring[2];
    v32qi firstRRingOffset;
};

/// Last message decoded by AVX2 path.
extern union ScoringDecodedMessage decodedMsgPartAvx2;

extern struct PathLookupAvx2_t PathLookupAvx2;

extern enigma_cipher_function_t enigma_cipher_DecoderLookupAvx2;

#endif
