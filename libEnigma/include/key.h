#ifndef KEY_H
#define KEY_H

#include <stdint.h>

#include "global.h"
#include "config/types.h"
#include "character_encoding.h"

/* PermutationMap
 * * * * * * * * */
union PermutationMap_t
{
    union v32_echar whole;

    union v16_echar half[2];

    struct echar letters[32];
};

static inline
void FixPermutationMapTail(union PermutationMap_t* mapping){
    int k = 26;
    for(; k < 32; k++)
    {
        mapping->letters[k] = mapping->letters[k-26];
    }
}

struct PermutationMap26 {
    struct echar map[ 26 ];
};

__attribute__((optimize("unroll-loops")))
static inline
void Fill0To25( text_t array[26] )
{
    int32_t* arrayInt = (int32_t*)array;
    int i = 0;
    int val  = 0x03020100;
    int step = 0x04040404;
    for( ; i * 4 < 26 - 3; i++ ){
        arrayInt[i] = val;
        val += step;
    }
    int16_t* arrayShort = (int16_t*)array;
    arrayShort[12] = 0x1918;
}

static inline
void Fill0To25_echar( struct echar array[26] ){
    Fill0To25( &array[0].encoded );
}


struct Ringstellung
{
    struct echar_delta m, r;
};
STATIC_ASSERT( sizeof( struct Ringstellung ) == 2, "Ringstellung should be only 2 bytes long." );


/* RingsState
 * * * * * * */
struct RingsState
{
    struct echar_delta g, l, m, r;
};
STATIC_ASSERT( sizeof( struct RingsState ) == 4, "4" );

enum ring_type_enum {
    RingType_None = 0,
    RingType_1 = 1,
    RingType_2 = 2,
    RingType_3 = 3,
    RingType_4 = 4,
    RingType_5 = 5,
    RingType_6 = 6,
    RingType_7 = 7,
    RingType_8 = 8,
    __RingType__enforce_signed_type = -1,
} __attribute__(( packed ));
STATIC_ASSERT( sizeof( enum ring_type_enum ) == 1, "ring_type_enum should fit in a single byte." );


struct RingType {
    enum ring_type_enum type;
};
STATIC_ASSERT( sizeof( struct RingType ) == 1, "RingType should be a single byte." );

char
RingType_to_ALPHA( struct RingType rt );


enum greek_ring_type_enum {
    GreekRingType_None = 0,
    GreekRingType_Beta = 9,
    GreekRingType_Gamma = 10,
    __GreekRingType__enforce_signed_type = -1,
} __attribute__(( packed ));
STATIC_ASSERT( sizeof( enum greek_ring_type_enum ) == 1, "greek_ring_type_enum should be one byte." );


struct GreekRingType {
    enum greek_ring_type_enum type;
};
STATIC_ASSERT( sizeof( struct GreekRingType ) == 1, "GreekRingType should be a single byte." );

char
GreekRingType_to_ALPHA( struct GreekRingType rt );


struct RingTypes 
{
    struct GreekRingType g;
    struct RingType l, m, r;
};


enum ukw_type_enum {
    UkwType_A,
    UkwType_B,
    UkwType_C,
    UkwType_B_Thin,
    UkwType_C_Thin,
    __UkwType__enforce_signed_type = -1,
} __attribute__(( packed ));
STATIC_ASSERT( sizeof( enum ukw_type_enum ) == 1, "ukw_type_enum should be one byte." );


struct UkwType {
    enum ukw_type_enum type;
};
STATIC_ASSERT( sizeof( struct UkwType ) == 1, "UkwType should be one byte." );


static inline
enum comparison_result
UkwType_cmp( struct UkwType l, struct UkwType r ){
    if( l.type == r.type ) return cmp_equal;
    if( l.type >  r.type ) return cmp_greater;
    else return cmp_less;
}

char
UkwType_to_ALPHA( struct UkwType u );

/* Model
 * * * * */
 enum ModelType_t{
    EnigmaModel_Error   = -1,
    EnigmaModel_H       =  0,
    EnigmaModel_M3      =  1,
    EnigmaModel_M4      =  2,
 } __attribute__(( packed ));

/* Key
 * * * */
struct Key {
    ALIGNED_16(union PermutationMap_t stbrett);
    struct RingsState mesg; ///< message settings
    struct RingTypes  slot; ///< Contains numbers of rings in slots. /* greek, left, middle, right slot */
    struct Ringstellung ring; ///< ringstellungen
    struct UkwType ukwnum;
    enum ModelType_t model;
    struct PermutationMap26 sf; //< swapped/free letters
    int count;      /* number of swapped letters */
    int score;      /* hillclimbing score */
};

int init_key_default( struct Key *key, enum ModelType_t model);
int init_key_low( struct Key *key, enum ModelType_t model);

PURE_FUNCTION
enum comparison_result
keycmp( const struct Key* k1, const struct Key* k2 );


PURE_FUNCTION
bool
Key_equ( const struct Key* k1, const struct Key* k2 );


#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
