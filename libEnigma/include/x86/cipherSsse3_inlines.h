#pragma once

#include "error.h"
#include "dict.h"
#include "cipherSsse3.h"
#include "character_encoding.h"

#include "echar/echar_ssse3.h"

static inline
union v16_echar
DecodeBiteForwardCommonSsse3( union v16_echar bite, union v16_echar_delta rRingOffset, const struct Key* const restrict key ){
    // stbrett forward
    bite = v16_echar_map( bite, key->stbrett );
    // right ring forward
    bite = v16_echar_add_delta( bite, rRingOffset );
    bite = v16_echar_map( bite, PathLookupSsse3.r_ring[0] );
    bite = v16_echar_sub_delta( bite, rRingOffset );
    return bite;
}

static inline
union v16_echar
DecodeBiteMaskedPartSsse3( union v16_echar predecodedBite, int lookupNumber ) {    
    // m+l rings and ukw
    union v16_echar masked = v16_echar_map( predecodedBite, PathLookupSsse3.lookups[lookupNumber].mapping );
    masked.vector &= PathLookupSsse3.lookups[lookupNumber].mask;
    return masked;
}

static inline
union v16_echar
DecodeBiteBackwardCommonSsse3( union v16_echar bite, union v16_echar_delta rRingOffset, const struct Key* const restrict key ) {
    // right ring backwards
    bite = v16_echar_add_delta( bite, rRingOffset );
    bite = v16_echar_map( bite, PathLookupSsse3.r_ring[1] );
    bite = v16_echar_sub_delta( bite, rRingOffset );
    //stbrett backwards
    bite = v16_echar_map( bite, key->stbrett );
    return bite;
}

static inline
union v16_echar
CombineMaskedPartsSsse3( union v16_echar l, union v16_echar r ){
    return (union v16_echar){ .vector = l.vector | r.vector };
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
        union v16_echar cBite = {};
        lookupNumber += lookupsToNextBite;
        union v16_echar currentBite = ciphertext.vector16[messageBite];
        union v16_echar predecoded  = DecodeBiteForwardCommonSsse3( currentBite, currentRRingOffset, key );

        switch( lookupsToNextBite ) {
        case 4:
            cBite  = DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 4 );
            FALLTHROUGH();
        case 3:
            cBite = CombineMaskedPartsSsse3( cBite, DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 3 ) );
            FALLTHROUGH();
        case 2:
            cBite = CombineMaskedPartsSsse3( cBite, DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 2 ) );
            FALLTHROUGH();
        case 1:
            cBite = CombineMaskedPartsSsse3( cBite, DecodeBiteMaskedPartSsse3( predecoded, lookupNumber - 1 ) );
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
        f[ echar_0_based_index( msg->plain[i] ) ]++;
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
v16qi MOVDQU( const v16qi* p ){
    return __builtin_ia32_loaddqu( (char*) p );
}

__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeTriscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0;
    int i;
    for( i = 0; i * 16 + 15 < len - 2; ++i ) {
        v16qi a = msg->vector16[i].vector;
        v8hi aLo, aHi;
        Unpack_v16qi( a, &aLo, &aHi );
        aLo *= 32 * 32;
        aHi *= 32 * 32;
        const void* a_addr = &msg->vector16[i].vector ;
        v16qi b = MOVDQU( a_addr + 1 );
        v8hi bLo, bHi;
        Unpack_v16qi( b, &bLo, &bHi );
        bLo *= 32;
        bHi *= 32;
        v16qi c = MOVDQU( a_addr + 2 );
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
    i *= 16;
    for( ; i < len - 2; ++i ) {
        score += tridict[ echar_0_based_index( msg->plain[i] ) ]
                        [ echar_0_based_index( msg->plain[i + 1] ) ]
                        [ echar_0_based_index( msg->plain[i + 2] ) ];
    }
    return score;
}


__attribute__ ((optimize("unroll-loops")))
static inline
int ComputeBiscoreFromDecodedMsgSse2( const union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0;
    int i;
    for( i = 0; i * 16 + 15 < len - 1; ++i ) {
        v16qi a = msg->vector16[i].vector;
        v8hi aLo, aHi;
        Unpack_v16qi( a, &aLo, &aHi );
        aLo *= 32;
        aHi *= 32;
        const void* a_addr = &msg->vector16[i].vector;
        v16qi b = MOVDQU( a_addr + 1 );
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
    i *= 16;
    for( ; i < len - 1; ++i ) {
        score += bidict[ echar_0_based_index( msg->plain[i] ) ]
                       [ echar_0_based_index( msg->plain[i + 1] ) ];
    }
    return score;
}
