#ifndef CIPHER_SSSE3_INLINES_H_INCLUDED
#define CIPHER_SSSE3_INLINES_H_INCLUDED

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
