/*
 * This file is part of enigma-suite, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2016 Alex Shovkoplyas (VE3NEA)
 *
 */

#include <stdint.h>

#include "stbrett/ve3nea_optimizer.h"
#include "stecker.h"


static void     OptimizeIcscore2 ( const struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf );
static void     OptimizeUniscore2( const struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf );
static uint32_t OptimizeTriscore2( const struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf );

typedef int ( score_f) ( const struct Key* restrict key, scoreLength_t length );
static uint32_t score_optimizer_loop ( const struct echar var[26], struct Key* const ckey, int len, score_f* score, uint32_t start_score );


int
stbrett_optimize_ve3nea(
    const struct echar var[26],
    struct Key* ckey,
    int len,
    const enigma_score_function_t* scoring )
{
    OptimizeIcscore2 ( var, ckey, len, scoring );
    OptimizeUniscore2( var, ckey, len, scoring );
    uint32_t bestscore = OptimizeTriscore2( var, ckey, len, scoring );
    get_stecker( ckey );
    return bestscore;
}


NO_INLINE
static uint32_t score_optimizer_loop (
      const struct echar var[26]
    , struct Key* const ckey
    , int len
    , score_f* score
    , uint32_t start_score ) {
    union PermutationMap_t* const stbr = &ckey->stbrett;
    union PermutationMap_t stbr_copy = *stbr;
    uint32_t best_score  = start_score;
    for ( size_t p = 0; p < 25; p++ ) {
        for ( size_t q = p + 1; q < 26; q++ ) {
            const struct echar
                i = var[p],
                k = var[q],
                x = stbr->letters[ echar_0_based_index( i ) ],
                z = stbr->letters[ echar_0_based_index( k ) ];
            if ( echar_eq( x, k ) ){
                stbr->letters[ echar_0_based_index( i ) ] = i;
                stbr->letters[ echar_0_based_index( k ) ] = k;
            }
            else {
                if ( echar_neq( x, i ) ){
                    stbr->letters[ echar_0_based_index( i ) ] = i;
                    stbr->letters[ echar_0_based_index( x ) ] = x;
                };
                if ( echar_neq( z, k ) ){
                    stbr->letters[ echar_0_based_index( k ) ] = k;
                    stbr->letters[ echar_0_based_index( z ) ] = z;
                };
                stbr->letters[ echar_0_based_index( i ) ] = k;
                stbr->letters[ echar_0_based_index( k ) ] = i;
            }
            uint32_t s = score( ckey, len );
            if (s > best_score) {
                best_score = s;
                stbr_copy = *stbr;
            }
            else {
                *stbr = stbr_copy;
            }
        }
    }
    return best_score;
}

void OptimizeIcscore2 (
      const struct echar var[26]
    , struct Key* const ckey
    , int len
    , const enigma_score_function_t* const sf ) {
    // restult of icscore is stored in r/eax reagardless of int size
    score_f* ic_to_int = (score_f*) sf->icscore;
    uint16_t first_ic = sf->icscore( ckey, len );
    score_optimizer_loop( var, ckey, len, ic_to_int, first_ic );
}

void OptimizeUniscore2 (
      const struct echar var[26]
    , struct Key* const ckey
    , int len
    , const enigma_score_function_t* const sf ) {
    int first_uni = sf->uniscore( ckey, len );
    score_optimizer_loop( var, ckey, len, sf->uniscore, first_uni );
}

uint32_t OptimizeTriscore2 (
      const struct echar var[26]
    , struct Key* const ckey
    , int len
    , const enigma_score_function_t* const sf ) {
    uint32_t jbestscore;
    uint32_t besttri = sf->triscore( ckey, len );
    do {
        jbestscore = besttri;
        besttri = score_optimizer_loop( var, ckey, len, sf->triscore, besttri );
    } while (besttri > jbestscore);
    assert( besttri == jbestscore );
    return jbestscore;
}
