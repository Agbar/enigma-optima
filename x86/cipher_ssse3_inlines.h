#ifndef CIPHER_SSSE3_INLINES_H_INCLUDED
#define CIPHER_SSSE3_INLINES_H_INCLUDED

inline
v16qi PermuteV16qi(const PermutationMap_t* map, v16qi vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x1F;
    vec += (char) 0x70; // For every byte push value of bit[4] to bit[7].
    v16qi ret1 = __builtin_ia32_pshufb128( map->half[0], vec );
    v16qi ret2 = __builtin_ia32_pshufb128( map->half[1], vec ^ (char) 0x80 );
    return ret1 | ret2;
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
