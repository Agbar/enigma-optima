#ifndef CIPHERTEXT_SSSE3_H_INCLUDED
#define CIPHERTEXT_SSSE3_H_INCLUDED
#include "..\cipher.h"
#include "..\score.h"

struct LookupChunk_t {
        PermutationMap_t mapping;
        v16qi rRingOffset;
        v16qi mask;
};

struct PathLookupSsse3_t
{
    struct LookupChunk_t lookups[24];
    uint_least16_t nextBite[24];
    uint_least8_t lookupsCounter;
    // mapping forward and reverse
    PermutationMap_t r_ring[2];
};

/// Last message decoded by SSSE3 path.
union ScoringDecodedMessage decodedMessageSsse3;

extern struct PathLookupSsse3_t PathLookupSsse3;

extern enigma_cipher_function_t enigma_cipher_decoder_lookup_ssse3;

//v16qi enigma_cipher_decode_ssse3(int text_bite, int lookupNumber, const Key* const restrict key);

inline
v16qi enigma_cipher_decode_ssse3( int biteNumber, int lookupNumber, const Key* const restrict key )
{
    v16qi bite = ciphertext.vector16[biteNumber];

    // stbrett forward
    bite = __builtin_shuffle( key->stbrett.half[0], key->stbrett.half[1], bite );

    const struct LookupChunk_t* const restrict lookup = &PathLookupSsse3.lookups[lookupNumber];

    bite += lookup->rRingOffset;
    bite -= ( bite >= 26 ) & 26;
    bite = __builtin_shuffle( PathLookupSsse3.r_ring[0].half[0], PathLookupSsse3.r_ring[0].half[1], bite );
    bite -= lookup->rRingOffset;
    bite += ( bite < 0 ) & 26;

    // m+l rings and ukw
    bite = __builtin_shuffle( lookup->mapping.half[0],  lookup->mapping.half[1],  bite );

    // right ring backwards
    bite += lookup->rRingOffset;
    bite -= ( bite >= 26 ) & 26;
    bite = __builtin_shuffle( PathLookupSsse3.r_ring[1].half[0], PathLookupSsse3.r_ring[1].half[1], bite );
    bite -= lookup->rRingOffset;
    bite += ( bite < 0 ) & 26;

    //stbrett backwards
    bite = __builtin_shuffle( key->stbrett.half[0], key->stbrett.half[1], bite );

    return bite & PathLookupSsse3.lookups[lookupNumber].mask;
}

#endif
