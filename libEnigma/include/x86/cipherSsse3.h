#ifndef CIPHERTEXT_SSSE3_H_INCLUDED
#define CIPHERTEXT_SSSE3_H_INCLUDED

#include "cipher.h"
#include "score.h"

struct LookupChunk_t {
        union PermutationMap_t mapping;
        v16qi mask;
};

struct PathLookupSsse3_t
{
    struct LookupChunk_t lookups[24];
    uint_least16_t nextBite[24];        ///< First lookup number in next bite.
    // mapping forward and reverse
    union PermutationMap_t r_ring[2];
    v16qi firstRRingOffset;
};

/// Last message decoded by SSSE3 path.
extern union ScoringDecodedMessage decodedMsgPartSsse3;

extern struct PathLookupSsse3_t PathLookupSsse3;

extern enigma_cipher_function_t enigma_cipher_decoder_lookup_ssse3;

#endif
