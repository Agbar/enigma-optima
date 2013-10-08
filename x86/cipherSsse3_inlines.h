#ifndef CIPHER_SSSE3_INLINES_H_INCLUDED
#define CIPHER_SSSE3_INLINES_H_INCLUDED

#include "..\dict.h"
#include "cipherSsse3.h"

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
v16qi enigma_cipher_decode_ssse3( int biteNumber, int lookupNumber, v16qi rRingOffset, const Key* const restrict key )
{
    v16qi bite = ciphertext.vector16[biteNumber];

    // stbrett forward
    bite = PermuteV16qi ( &key->stbrett, bite );

    const struct LookupChunk_t* const restrict lookup = &PathLookupSsse3.lookups[lookupNumber];

    bite = AddMod26_v16qi( bite, rRingOffset );
    bite = PermuteV16qi( &PathLookupSsse3.r_ring[0], bite );
    bite = SubMod26_v16qi( bite, rRingOffset );
    // m+l rings and ukw
    bite = PermuteV16qi( &lookup->mapping,  bite );
    // right ring backwards
    bite = AddMod26_v16qi( bite, rRingOffset );
    bite = PermuteV16qi( &PathLookupSsse3.r_ring[1], bite );
    bite = SubMod26_v16qi( bite, rRingOffset );
    //stbrett backwards
    bite = PermuteV16qi( &key->stbrett, bite );

    return bite & PathLookupSsse3.lookups[lookupNumber].mask;
}

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
inline
void DecodeScoredMessagePartSsse3( const const Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;
    v16qi currentRRingOffset = PathLookupSsse3.firstRRingOffset;
    while( messageBite < ( len + 15 ) / 16 )
    {
        /* Worst case:
         *  P0123456789ABCDE    R-ring position (turnovers on 12 & 25, coded C & P)
         *  0123456789ABCDEF    characters in bite
         *  |||           |
         *  |||           +-  turnover on second notch of R ring
         *  ||+- turnover caused by M-ring (turning L- & M- rings).
         *  |+-- turnover setting M-ring to turnover position
         *  +--- last character from previous bite
         *
         *  In the worst case there are 4 lookups per bite.
         */
        uint_least16_t lookupsToNextBite = PathLookupSsse3.nextBite[messageBite] - lookupNumber;
        v16qi cBite = {0};
        lookupNumber += lookupsToNextBite;
        switch( lookupsToNextBite ) {
        case 4:
            cBite  = enigma_cipher_decode_ssse3( messageBite, lookupNumber - 4, currentRRingOffset, key );
        case 3:
            cBite |= enigma_cipher_decode_ssse3( messageBite, lookupNumber - 3, currentRRingOffset, key );
        case 2:
            cBite |= enigma_cipher_decode_ssse3( messageBite, lookupNumber - 2, currentRRingOffset, key );
        case 1:
            cBite |= enigma_cipher_decode_ssse3( messageBite, lookupNumber - 1, currentRRingOffset, key );
            break;
        default:
            exit(5);
        }
        // store whole decoded bite
        output -> vector16[messageBite] = cBite;
        messageBite++;
        currentRRingOffset = AddMod26_v16qi_int8( currentRRingOffset, 16 );
    }
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
inline
double ComputeIcscoreFromDecodedMsgSsse3( union ScoringDecodedMessage* msg, scoreLength_t len ){
    uint8_t ALIGNED_32(f[32]) = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[msg->plain[i]]++;
    }

    v16qi* const v = (v16qi*) f; // it makes v16hi[2];
    v16qi v0 = v[0];
    v16qi v1 = v[1];
    v16qi minusOne = {0};
    minusOne = ~minusOne;
    // short result[i] = v0[2*i] * ( v0[2*i] + minusOne ) + v0[2*i+1] * ( v0[2*i+1] + minusOne );
    v8hi foo = __builtin_ia32_pmaddubsw128( v0, v0 + minusOne );
    v8hi bar = __builtin_ia32_pmaddubsw128( v1, v1 + minusOne );
    foo += bar;
    foo = __builtin_ia32_phaddw128( foo, foo ); // [0+1, 2+3, 4+5, 6+7, ...]
    foo = __builtin_ia32_phaddw128( foo, foo ); // [0+1+2+3, 4+5+6+7, ...]
    foo = __builtin_ia32_phaddw128( foo, foo );

    STATIC_ASSERT ( UINT16_MAX > CT * CT, "uint16_t is to narrow for current CT value. Use ie. uint32_t." );
    uint16_t sum = foo[0];
    return ( double )sum / ( len * ( len - 1 ) );
}

#endif
