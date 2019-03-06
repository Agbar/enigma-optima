#ifndef CIPHER_H
#define CIPHER_H

#include <stdint.h>
#include <stdio.h>

#include "ciphertext.h"
#include "common.h"
#include "cpu.h"
#include "global.h"
#include "key.h"
#include "ModMath.h"
#include "config/array_sizes.h"
#include "config/types.h"
#include "character_encoding.h"

struct Turnovers_t {
    struct turnover r,
     r2,
     m,
     m2;
};

PURE_FUNCTION
int scrambler_state(const struct Key *restrict key, int len);

typedef void (*enigma_prepare_decoder_lookup_function_pt) (const struct Key *key, int len);

typedef struct _enigma_cipher_function_t
{
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup_M_H3;
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup_ALL;
} enigma_cipher_function_t;

void enigma_cipher_init(enigma_cpu_flags_t cpu, enum ModelType_t machine_type, enigma_prepare_decoder_lookup_function_pt* cf);

extern enigma_cipher_function_t enigma_cipher_decoder_lookup;
extern union PermutationMap_t path_lookup[CT];

union DoublePermutationMap {
    struct echar flat[ 26 * 2 ];
    struct PermutationMap26 dbl[ 2 ];
};

STATIC_ASSERT( 
    sizeof(union DoublePermutationMap) == 2 * sizeof(struct PermutationMap26) 
    , "Dense packing expected" );

static inline 
size_t double_index( struct echar in, struct echar_delta offset ){
    return echar_0_based_index( in ) + offset.delta;
}

/*
 * decoders common data
 *************************/
extern const struct turnover wal_turn[9];
extern const union DoublePermutationMap     wal[11];
extern const union DoublePermutationMap rev_wal[11];
extern const union DoublePermutationMap     ukw[ 5];

static inline
struct echar
decode( size_t offset,size_t index, const union PermutationMap_t* const stbrett );

static inline
v4piu decode4( size_t offset, size_t index, const union PermutationMap_t* const stbrett );

#ifdef __i386__

static inline
struct echar
decode( size_t offset,size_t index, const union PermutationMap_t* const stbrett )
{
    size_t c;
    asm(
        "movsb%z[c] %[ctext] + %c[offset] ( %[ind] ),    %[c]    \n\t"
        : [c]       "=&r"    ( c )
        : [ctext]   "m"     ( ciphertext )
        , [ind]     "r"     ( index )
        , [offset]  "i"     ( offset ));

    c = echar_0_based_index( stbrett->letters[c] );

    // path_lookup[Offset+Index][(Cx)]
    asm(
        "movsb%z[c] %[path_lookup] + %c[offset] * %c[ld]( %[index], %[c] ), %[c]    \n\t"
        : [c]           "+&r"   ( c )
        : [path_lookup] "m"     ( path_lookup )
        , [index]       "r"     ( index * LAST_DIMENSION )
        , [offset]      "i"     ( offset )
        , [ld]          "i"     ( LAST_DIMENSION ));

    return stbrett->letters[c];
}

static inline
v4piu decode4( size_t offset, size_t index, const union PermutationMap_t* const stbrett )
{
    size_t c;
    size_t d;
    size_t e;
    size_t f;
    asm(
        "movsb%z[c] %[ctext] + %c[offset] + 0( %[ind] ),    %[c]    \n\t"
        "movsb%z[d] %[ctext] + %c[offset] + 1( %[ind] ),    %[d]    \n\t"
        "movsb%z[e] %[ctext] + %c[offset] + 2( %[ind] ),    %[e]    \n\t"
        "movsb%z[f] %[ctext] + %c[offset] + 3( %[ind] ),    %[f]    \n\t"
        : [c]       "=&r"    ( c )
        , [d]       "=&r"    ( d )
        , [e]       "=&r"    ( e )
        , [f]       "=&r"    ( f )
        : [ctext]   "m"     ( ciphertext )
        , [ind]     "r"     ( index )
        , [offset]  "i"     ( offset ));

    c = echar_0_based_index( stbrett->letters[c] );
    d = echar_0_based_index( stbrett->letters[d] );
    e = echar_0_based_index( stbrett->letters[e] );
    f = echar_0_based_index( stbrett->letters[f] );

    // path_lookup[Offset+Index][(Cx)]
    asm(
        "movsb%z[c] %[path_lookup] + ( %c[offset] + 0 ) * %c[ld]( %[index], %[c] ),  %[c]    \n\t"
        "movsb%z[d] %[path_lookup] + ( %c[offset] + 1 ) * %c[ld]( %[index], %[d] ),  %[d]    \n\t"
        "movsb%z[e] %[path_lookup] + ( %c[offset] + 2 ) * %c[ld]( %[index], %[e] ),  %[e]    \n\t"
        "movsb%z[f] %[path_lookup] + ( %c[offset] + 3 ) * %c[ld]( %[index], %[f] ),  %[f]    \n\t"

        : [c]           "+&r"   ( c )
        , [d]           "+&r"   ( d )
        , [e]           "+&r"   ( e )
        , [f]           "+&r"   ( f )
        : [path_lookup] "m"     ( path_lookup )
        , [index]       "r"     ( index * LAST_DIMENSION )
        , [offset]      "i"     ( offset )
        , [ld]          "i"     ( LAST_DIMENSION ));

    v4piu ret = { echar_0_based_index( stbrett->letters[c] )
                , echar_0_based_index( stbrett->letters[d] )
                , echar_0_based_index( stbrett->letters[e] )
                , echar_0_based_index( stbrett->letters[f] )
                };
    return ret;
}

#endif

#ifdef __amd64__

static inline
struct echar
decode( size_t offset,size_t index, const union PermutationMap_t* const stbrett )
{
    size_t c;
    asm(
        "movsb%z[c] %c[offset] ( %[ctext], %[ind] ),    %[c]    \n\t"

        : [c]       "=&r"   ( c )
        : [ctext]   "r"     ( &ciphertext )
        , [ind]     "r"     ( index )
        , [offset]  "i"     ( offset ));

    c = echar_0_based_index( stbrett->letters[c] );

    // path_lookup[Offset+Index][(Cx)]
    asm(
        "movsb%z[c] ( %c[offset] + 0 ) * %c[ld]( %[p_lookup_ind], %[c] ),  %[c]    \n\t"

        : [c]           "+&r"  ( c )
        : [p_lookup_ind]"r"    ( path_lookup + index )
        , [offset]      "i"     ( offset )
        , [ld]          "i"     ( LAST_DIMENSION ));

    return stbrett->letters[c];
}

static inline
v4piu decode4( size_t offset, size_t index, const union PermutationMap_t* const stbrett )
{
    size_t c;
    size_t d;
    size_t e;
    size_t f;
    asm(
        "movsb%z[c] %c[offset] + 0( %[ctext], %[ind] ),    %[c]    \n\t"
        "movsb%z[d] %c[offset] + 1( %[ctext], %[ind] ),    %[d]    \n\t"
        "movsb%z[e] %c[offset] + 2( %[ctext], %[ind] ),    %[e]    \n\t"
        "movsb%z[f] %c[offset] + 3( %[ctext], %[ind] ),    %[f]    \n\t"
        : [c]       "=&r"    ( c )
        , [d]       "=&r"    ( d )
        , [e]       "=&r"    ( e )
        , [f]       "=&r"    ( f )
        : [ctext]   "r"     ( &ciphertext )
        , [ind]     "r"     ( index )
        , [offset]  "i"     ( offset ));

    c = echar_0_based_index( stbrett->letters[c] );
    d = echar_0_based_index( stbrett->letters[d] );
    e = echar_0_based_index( stbrett->letters[e] );
    f = echar_0_based_index( stbrett->letters[f] );

    // path_lookup[Offset+Index][(Cx)]
    asm(
        "movsb%z[c] ( %c[offset] + 0 ) * %c[ld]( %[p_lookup_ind], %[c] ),  %[c]    \n\t"
        "movsb%z[d] ( %c[offset] + 1 ) * %c[ld]( %[p_lookup_ind], %[d] ),  %[d]    \n\t"
        "movsb%z[e] ( %c[offset] + 2 ) * %c[ld]( %[p_lookup_ind], %[e] ),  %[e]    \n\t"
        "movsb%z[f] ( %c[offset] + 3 ) * %c[ld]( %[p_lookup_ind], %[f] ),  %[f]    \n\t"

        : [c]           "+&r"  ( c )
        , [d]           "+&r"  ( d )
        , [e]           "+&r"  ( e )
        , [f]           "+&r"  ( f )
        : [p_lookup_ind]"r"    ( path_lookup + index )
        , [offset]      "i"     ( offset )
        , [ld]          "i"     ( LAST_DIMENSION ));

    v4piu ret = { echar_0_based_index( stbrett->letters[c] )
                , echar_0_based_index( stbrett->letters[d] )
                , echar_0_based_index( stbrett->letters[e] )
                , echar_0_based_index( stbrett->letters[f] )
                };
    return ret;
}

#endif

static inline
void Step1( int8_t* ringOffset )
{
    IncrementMod( ringOffset, 26 );
}

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
