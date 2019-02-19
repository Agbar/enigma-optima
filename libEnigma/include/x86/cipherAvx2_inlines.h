#pragma once

#include "error.h"
#include "cipherAvx2.h"

static inline
v32qi PermuteV32qi(const union PermutationMap_t* map, v32qi vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x3F;
    vec += ( char ) 0x70; // For every byte push value of bit[4] to bit[7].
    vec ^= ( v32qi ) _mm256_set_epi32( 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0, 0, 0, 0 );
    v32qi pMap = map->whole.vector;
    v32qi ret1 = __builtin_ia32_pshufb256( pMap, vec );
    pMap = ( v32qi ) __builtin_ia32_permti256( ( v4di )pMap ,( v4di )pMap , 1 ); //VPERM2I128
    v32qi ret2 = __builtin_ia32_pshufb256( pMap , vec ^ (char) 0x80 );
    return ret1 | ret2;
}

static inline
union v32_echar
DecodeBiteForwardCommonAvx2( union v32_echar bite, union v32_echar_delta rRingOffset, const struct Key* const restrict key ) {
    // stbrett forward
    bite.vector = PermuteV32qi ( &key->stbrett, bite.vector );
    // right ring forward
    bite.vector = AddMod26_v32qi( bite.vector, rRingOffset.vector );
    bite.vector = PermuteV32qi( &PathLookupAvx2.r_ring[0], bite.vector );
    bite.vector = SubMod26_v32qi( bite.vector, rRingOffset.vector );
    return bite;
}

static inline
union v32_echar
DecodeBiteMaskedPartAvx2( union v32_echar predecodedBite, int lookupNumber ) {
    v32qi bite = predecodedBite.vector;
    // m+l rings and ukw
    bite  = PermuteV32qi( &PathLookupAvx2.lookups[lookupNumber].mapping,  bite );
    bite &= PathLookupAvx2.lookups[lookupNumber].mask;
    return (union v32_echar){ .vector = bite };
}

static inline
union v32_echar
DecodeBiteBackwardCommonAvx2( union v32_echar bite, union v32_echar_delta rRingOffset, const struct Key* const key ) {
    // right ring backwards
    bite.vector = AddMod26_v32qi( bite.vector, rRingOffset.vector );
    bite.vector = PermuteV32qi( &PathLookupAvx2.r_ring[1], bite.vector );
    bite.vector = SubMod26_v32qi( bite.vector, rRingOffset.vector );
    //stbrett backwards
    bite.vector = PermuteV32qi( &key->stbrett, bite.vector );
    return bite;
}

static inline
union v32_echar
CombineMaskedPartsAvx2( union v32_echar l, union v32_echar r ){
    return (union v32_echar){ .vector = l.vector | r.vector };
}

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
void DecodeScoredMessagePartAvx2( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;
    union v32_echar_delta currentRRingOffset = PathLookupAvx2.firstRRingOffset;
    while( messageBite < ( len + 31 ) / 32 )
    {
        /* Worst case:
         *  P0123456789ABCDEFGHIJKLMNOP01234    R-ring position (turnovers on 12 & 25, coded C & P)
         *  0123456789ABCDEF0123456789ABCDEF    characters in bite
         *  |||           |            |
         *  |||           |            +
         *  |||           +-  turnover on second notch of R ring
         *  ||+- turnover caused by M-ring (turning L- & M- rings).
         *  |+-- turnover setting M-ring to turnover position
         *  +--- last character from previous bite
         *
         *  In the worst case there are 5 lookups per bite.
         */
        uint_least16_t lookupsToNextBite = PathLookupAvx2.nextBite[messageBite] - lookupNumber;
        union v32_echar cBite = {};
        lookupNumber += lookupsToNextBite;
        union v32_echar currentBite = ciphertext.vector32[messageBite];
        union v32_echar predecoded = DecodeBiteForwardCommonAvx2( currentBite, currentRRingOffset, key );

        switch( lookupsToNextBite ) {
        case 5:
            cBite  = DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 5 );
            FALLTHROUGH();
        case 4:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 4 ) );
            FALLTHROUGH();
        case 3:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 3 ) );
            FALLTHROUGH();
        case 2:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 2 ) );
            FALLTHROUGH();
        case 1:
            cBite = CombineMaskedPartsAvx2( cBite, DecodeBiteMaskedPartAvx2( predecoded, lookupNumber - 1 ) );
            break;
        default:
            exit_d(5);
            UNREACHABLE();
        }
        cBite = DecodeBiteBackwardCommonAvx2( cBite, currentRRingOffset, key );
        // store whole decoded bite
        output -> vector32[messageBite] = cBite.vector;
        messageBite++;
        currentRRingOffset = v32_echar_delta_rot_32( currentRRingOffset );
    }
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
uint16_t ComputeIcscoreFromDecodedMsgAvx2( union ScoringDecodedMessage* msg, scoreLength_t len ){
    ALIGNED_32( uint8_t f[32] ) = {0};
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

    uint16_t sum = 256 *( high[0] + high[4] + high[8] + high[12] ) + low[0] + low[4] + low[8] + low[12];
    return sum;
}
