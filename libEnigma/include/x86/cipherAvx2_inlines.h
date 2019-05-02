#pragma once

#include "error.h"
#include "cipherAvx2.h"
#include "echar/echar_avx2.h"

static inline
union v32_echar
DecodeBiteForwardCommonAvx2( union v32_echar bite, union v32_echar_delta rRingOffset, const struct Key* const restrict key ) {
    // stbrett forward
    bite = v32_echar_map ( bite, key->stbrett );
    // right ring forward
    bite = v32_echar_add_delta( bite, rRingOffset );
    bite = v32_echar_map( bite, PathLookupAvx2.r_ring[0] );
    bite = v32_echar_sub_delta( bite, rRingOffset );
    return bite;
}

static inline
union v32_echar
DecodeBiteMaskedPartAvx2( union v32_echar predecodedBite, int lookupNumber ) {
    // m+l rings and ukw
    union v32_echar masked = v32_echar_map( predecodedBite, PathLookupAvx2.lookups[lookupNumber].mapping );
    masked.vector &= PathLookupAvx2.lookups[lookupNumber].mask;
    return masked;
}

static inline
union v32_echar
DecodeBiteBackwardCommonAvx2( union v32_echar bite, union v32_echar_delta rRingOffset, const struct Key* const key ) {
    // right ring backwards
    bite = v32_echar_add_delta( bite, rRingOffset );
    bite = v32_echar_map( bite, PathLookupAvx2.r_ring[1] );
    bite = v32_echar_sub_delta( bite, rRingOffset );
    //stbrett backwards
    bite = v32_echar_map( bite, key->stbrett );
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
        output -> vector32[messageBite] = cBite;
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
        f[ echar_0_based_index( msg->plain[i] ) ]++;
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
