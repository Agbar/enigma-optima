#ifndef CIPHER_AVX2_INLINES_H_INCLUDED
#define CIPHER_AVX2_INLINES_H_INCLUDED

inline
v32qi PermuteV32qi(const PermutationMap_t* map, v32qi vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x3F;
    vec += ( char ) 0x70; // For every byte push value of bit[4] to bit[7].
    vec ^= ( v32qi ) _mm256_set_epi32( 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0, 0, 0, 0 );
    v32qi pMap = map->whole;
    v32qi ret1 = __builtin_ia32_pshufb256( pMap, vec );
    pMap = ( v32qi ) __builtin_ia32_permti256( ( v4di )pMap ,( v4di )pMap , 1 ); //VPERM2I128
    v32qi ret2 = __builtin_ia32_pshufb256( pMap , vec ^ (char) 0x80 );
    return ret1 | ret2;
}

inline
v32qi DecodeBiteAvx2( int biteNumber, int lookupNumber, const Key* const restrict key )
{
    v32qi bite = ciphertext.vector32[biteNumber];

    // stbrett forward
    bite = PermuteV32qi ( &key->stbrett, bite );

    const struct LookupChunkAvx2_t* const restrict lookup  = &PathLookupAvx2.lookups[lookupNumber];

    bite += lookup->rRingOffset;
    bite -= ( bite >= 26 ) & 26;
    bite = PermuteV32qi( &PathLookupAvx2.r_ring[0], bite );
    bite -= lookup->rRingOffset;
    bite += ( bite < 0 ) & 26;

    // m+l rings and ukw
    bite = PermuteV32qi( &lookup->mapping,  bite );

    // right ring backwards
    bite += lookup->rRingOffset;
    bite -= ( bite >= 26 ) & 26;
    bite = PermuteV32qi( &PathLookupAvx2.r_ring[1], bite );
    bite -= lookup->rRingOffset;
    bite += ( bite < 0 ) & 26;

    //stbrett backwards
    bite = PermuteV32qi( &key->stbrett, bite );

    return bite & PathLookupAvx2.lookups[lookupNumber].mask;
}

#endif
