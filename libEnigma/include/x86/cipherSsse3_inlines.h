#pragma once

#include "error.h"
#include "dict.h"
#include "cipherSsse3.h"

static inline
v16qi PermuteV16qi(const union PermutationMap_t* map, v16qi vec ){
    /* Following line is needed to behave like __builtin_shuffle for all inputs and still being
    faster, but our data is always in interval [0,25] = [0,0x1A). */
    // vec &= 0x1F;
    vec += (char) 0x70; // For every byte push value of bit[4] to bit[7].
    v16qi ret1 = __builtin_ia32_pshufb128( map->half[0].vector, vec );
    v16qi ret2 = __builtin_ia32_pshufb128( map->half[1].vector, vec ^ (char) 0x80 );
    return ret1 | ret2;
}


static inline
v16qi DecodeBiteForwardCommonSsse3( v16qi bite, union v16_echar_delta rRingOffset, const struct Key* const restrict key ){
    // stbrett forward
    bite = PermuteV16qi ( &key->stbrett, bite );
    // right ring forward
    bite = AddMod26_v16qi( bite, rRingOffset.vector );
    bite = PermuteV16qi( &PathLookupSsse3.r_ring[0], bite );
    bite = SubMod26_v16qi( bite, rRingOffset.vector );
    return bite;
}

static inline
v16qi DecodeBiteMaskedPartSsse3( v16qi predecodedBite, int lookupNumber ) {
    v16qi bite = predecodedBite;
    // m+l rings and ukw
    bite  = PermuteV16qi( &PathLookupSsse3.lookups[lookupNumber].mapping,  bite );
    bite &= PathLookupSsse3.lookups[lookupNumber].mask;
    return bite;
}

static inline
v16qi DecodeBiteBackwardCommonSsse3( v16qi bite, union v16_echar_delta rRingOffset, const struct Key* const restrict key ) {
    // right ring backwards
    bite = AddMod26_v16qi( bite, rRingOffset.vector );
    bite = PermuteV16qi( &PathLookupSsse3.r_ring[1], bite );
    bite = SubMod26_v16qi( bite, rRingOffset.vector );
    //stbrett backwards
    bite = PermuteV16qi( &key->stbrett, bite );
    return bite;
}

__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
void DecodeScoredMessagePartSsse3( const struct Key* const restrict key, int len, union ScoringDecodedMessage* output )
{
    uint16_t messageBite  = 0;
    uint_least16_t lookupNumber = 0;
    union v16_echar_delta currentRRingOffset = PathLookupSsse3.firstRRingOffset;
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
        v16qi currentBite = ciphertext.vector16[messageBite].vector;
        v16qi predecoded  = DecodeBiteForwardCommonSsse3( currentBite, currentRRingOffset, key );

        switch( lookupsToNextBite ) {
        case 4:
            cBite  = DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 4 );
            FALLTHROUGH();
        case 3:
            cBite |= DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 3 );
            FALLTHROUGH();
        case 2:
            cBite |= DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 2 );
            FALLTHROUGH();
        case 1:
            cBite |= DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 1 );
            break;
        default:
            exit_d(5);
            UNREACHABLE();
        }
        cBite = DecodeBiteBackwardCommonSsse3( cBite, currentRRingOffset, key );
        // store whole decoded bite
        output -> vector16[messageBite] = cBite;
        messageBite++;
        currentRRingOffset = v16_echar_delta_rot_16( currentRRingOffset );
    }
}

__attribute__ ((optimize("unroll-loops")))
__attribute__ ((optimize("unroll-loops,sched-stalled-insns=0,sched-stalled-insns-dep=16")))
static inline
uint16_t ComputeIcscoreFromDecodedMsgSsse3( union ScoringDecodedMessage* msg, scoreLength_t len ){
    uint8_t ALIGNED_32( f[32] ) = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[msg->plain[i]]++;
    }
    v16qi* const v = ( v16qi* ) f; // it makes v16hi[2];
    // short result[i] = v0[2*i] * ( v0[2*i] + minusOne ) + v0[2*i+1] * ( v0[2*i+1] + minusOne );
    v8hi foo = __builtin_ia32_pmaddubsw128( v[0], v[0] + -1 );
    v8hi bar = __builtin_ia32_pmaddubsw128( v[1], v[1] + -1 );
    foo += bar;
    v8hi hexFF00 = ( v8hi )_mm_set1_epi16( 0xFF00 );

#if defined( __AVX__ )
    asm(
        "vpandn %[hi], %[mask], %[lo]\n\t"
        "vpand  %[hi], %[mask], %[hi]\n\t" :
        [hi] "+x" (foo), [lo] "=&x" (bar) :
        [mask]"x" (hexFF00)
    );
#else
    v8hi temp = temp; //disable uninitialized warning
    asm(
        "movdqa %[hi],      %[tmp]\n\t"
        "pand   %[lo],      %[hi]\n\t"
        "pandn  %[tmp],     %[lo]\n\t" :
        [hi] "+x" ( foo ), [lo] "=x" ( bar ) :
        "1" ( hexFF00 ), [tmp] "x" ( temp )
    );
#endif

    v16qi zero = {0};
    v8hi high =  ( v8hi ) __builtin_ia32_psadbw128( ( v16qi ) foo, zero );
    v8hi low  =  ( v8hi ) __builtin_ia32_psadbw128( ( v16qi ) bar, zero );

    uint16_t sum = ( high[0] + high[4] )* 256 + low[0] + low[4];

    return sum;
}

static inline
void Unpack_v16qi( v16qi in, v8hi* lo, v8hi *hi ){
    v16qi zero = { 0 };
    *lo = (v8hi) __builtin_ia32_punpcklbw128 ( in, zero );
    *hi = (v8hi) __builtin_ia32_punpckhbw128 ( in, zero );
}

static inline
void Unpack_v8hi( v8hi in, v4si* lo, v4si* hi ){
    v8hi zero = { 0 };
    *lo = (v4si) __builtin_ia32_punpcklwd128( in, zero );
    *hi = (v4si) __builtin_ia32_punpckhwd128( in, zero );
}

static inline
v16qi MOVDQU( v16qi* p ){
    return __builtin_ia32_loaddqu( (char*) p );
}

__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeTriscoreFromDecodedMsgSse2( union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0;
    int i;
    for( i = 0; i + 15 < len - 2; i += 16 ) {
        v16qi a = *(v16qi*)( msg->plain + i );
        v8hi aLo, aHi;
        Unpack_v16qi( a, &aLo, &aHi );
        aLo *= 32 * 32;
        aHi *= 32 * 32;
        v16qi b = MOVDQU( (v16qi*) ( msg->plain + i + 1 ) );
        v8hi bLo, bHi;
        Unpack_v16qi( b, &bLo, &bHi );
        bLo *= 32;
        bHi *= 32;
        v16qi c = MOVDQU( (v16qi*) ( msg->plain + i + 2 ) );
        v8hi cLo, cHi;
        Unpack_v16qi( c, &cLo, &cHi );

        aLo += bLo + cLo;
        aHi += bHi + cHi;

        int j;
        for( j=0; j< 8; j++ ){
            score += *( ( dict_t* ) tridict + aLo[j] );
            score += *( ( dict_t* ) tridict + aHi[j] );
        }
    }
    for( ; i < len - 2; ++i ) {
        score += tridict[msg->plain[i]][msg->plain[i + 1]][msg->plain[i + 2]];
    }
    return score;
}


__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeBiscoreFromDecodedMsgSse2( union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0;
    int i;
    for( i = 0; i + 15 < len - 1; i += 16 ) {
        v16qi a = *(v16qi*)( msg->plain + i );
        v8hi aLo, aHi;
        Unpack_v16qi( a, &aLo, &aHi );
        aLo *= 32;
        aHi *= 32;
        v16qi b = MOVDQU( (v16qi*) ( msg->plain + i + 1 ) );
        v8hi bLo, bHi;
        Unpack_v16qi( b, &bLo, &bHi );

        aLo += bLo;
        aHi += bHi;

        int j;
        for( j=0; j< 8; j++ ){
            score += *( ( dict_t* ) bidict + aLo[j] );
            score += *( ( dict_t* ) bidict + aHi[j] );
        }
    }
    for( ; i < len - 1; ++i ) {
        score += bidict[msg->plain[i]][msg->plain[i + 1]];
    }
    return score;
}
