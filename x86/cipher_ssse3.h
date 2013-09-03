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
v16qi PermuteV16qi(const PermutationMap_t* map, v16qi vec ){
    return __builtin_shuffle( map->half[0], map->half[1], vec );
}

inline
v16qi enigma_cipher_decode_ssse3( int biteNumber, int lookupNumber, const Key* const restrict key )
{
    v16qi bite = ciphertext.vector16[biteNumber];

    // stbrett forward
    bite = PermuteV16qi ( &key->stbrett, bite );

    const struct LookupChunk_t* const restrict lookup = &PathLookupSsse3.lookups[lookupNumber];

    bite += lookup->rRingOffset;
    bite -= ( bite >= 26 ) & 26;
    bite = PermuteV16qi( &PathLookupSsse3.r_ring[0], bite );
    bite -= lookup->rRingOffset;
    bite += ( bite < 0 ) & 26;

    // m+l rings and ukw
    bite = PermuteV16qi( &lookup->mapping,  bite );

    // right ring backwards
    bite += lookup->rRingOffset;
    bite -= ( bite >= 26 ) & 26;
    bite = PermuteV16qi( &PathLookupSsse3.r_ring[1], bite );
    bite -= lookup->rRingOffset;
    bite += ( bite < 0 ) & 26;

    //stbrett backwards
    bite = PermuteV16qi( &key->stbrett, bite );

    return bite & PathLookupSsse3.lookups[lookupNumber].mask;
}

#endif
