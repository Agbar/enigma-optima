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
v32qi DecodeBiteAvx2( int biteNumber, int lookupNumber, v32qi rRingOffset, const Key* const restrict key )
{
    v32qi bite = ciphertext.vector32[biteNumber];

    // stbrett forward
    bite = PermuteV32qi ( &key->stbrett, bite );

    const struct LookupChunkAvx2_t* const restrict lookup  = &PathLookupAvx2.lookups[lookupNumber];

    bite = AddMod26_v32qi( bite, rRingOffset );
    bite = PermuteV32qi( &PathLookupAvx2.r_ring[0], bite );
    bite = SubMod26_v32qi( bite, rRingOffset );
    // m+l rings and ukw
    bite = PermuteV32qi( &lookup->mapping,  bite );
    // right ring backwards
    bite = AddMod26_v32qi( bite, rRingOffset );
    bite = PermuteV32qi( &PathLookupAvx2.r_ring[1], bite );
    bite = SubMod26_v32qi( bite, rRingOffset );
    //stbrett backwards
    bite = PermuteV32qi( &key->stbrett, bite );

    return bite & PathLookupAvx2.lookups[lookupNumber].mask;
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
inline
double ComputeIcscoreFromDecodedMsgAvx2( union ScoringDecodedMessage* msg, scoreLength_t len ){
    uint8_t f[32] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[msg->plain[i]]++;
    }

    v32qi v;
    /* Following asm is doing: v = *(v32qi*) f;
     * but it uses 256bit load which is fast in AVX2 processors. */
    asm (
        "VMOVDQA %1, %0" :
        "=x" (v) :
        "m" (f)
    );

    // short result[i] = v0[2*i] * ( v0[2*i] - 1 ) + v0[2*i+1] * ( v0[2*i+1] - 1 );
    v16hi foo   = __builtin_ia32_pmaddubsw256( v, v + -1 );
    v32qi zero  = { 0 };
    v16hi mask  = ( v16hi ) _mm256_set1_epi16( 0xFF00 );
    v16hi bar;
    asm(
        "vpandn %[hi], %[mask], %[lo]\n\t"
        "vpand  %[hi], %[mask], %[hi]\n\t" :
        [hi] "+x" ( foo ), [lo] "=&x" ( bar ) :
        [mask]"x" ( mask )
    );

    v16hi high = __builtin_ia32_psadbw256( ( v32qi ) ( foo ), zero );
    v16hi low  = __builtin_ia32_psadbw256( ( v32qi ) ( bar ), zero );

    STATIC_ASSERT ( UINT16_MAX > CT * CT, "uint16_t is to narrow for current CT value. Use ie. uint32_t." );
    uint16_t sum = 256 *( high[0] + high[4] + high[8] + high[12] ) + low[0] + low[4] + low[8] + low[12];
    return ( double )sum / ( len * ( len - 1 ) );
}


#endif
