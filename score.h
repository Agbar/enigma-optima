#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "cpu.h"
#include "key.h"
#include "cipher.h"

// skip 'restrict' C keyword in C++
#ifdef __cplusplus
# define restrict
#endif

typedef struct _enigma_score_function_t{
    int    (*triscore) (const Key* const restrict key, int len);
    int    (* biscore) (const Key* const restrict key, int len);
    double (* icscore) (const Key* const restrict key, int len);
    int    (*uniscore) (const Key* const restrict key, int len);
} enigma_score_function_t;

// Flags are used to allow combining.
enum EnigmaScoreFunctions_t {
    EnigmaSF_Default    = 0, ///< Default means not set.
    EnigmaSF_Original   = 1,
    EnigmaSF_Simple     = 2,
    EnigmaSF_Optimized  = 4,
    EnigmaSF_SSSE3      = 8,
};

// Initializes sf based on current cpu features.
/** \brief
 *
 * \param cpu int
 * \param sf enigma_score_function_t*
 * \return void
 *
 */
void enigma_score_init(enigma_cpu_flags_t cpu, enigma_score_function_t* restrict sf);


/** \brief Uses original code. Used as the best tested reference.
  */
extern enigma_score_function_t enigma_score_orig;


/** \brief Uses optimized version of original code. Gives minor performance improvement.
 */
extern enigma_score_function_t enigma_score_opt;

int get_triscore(const Key *key, int len);

union ScoringDecodedMessage
{
#if ((((CT)+15)/16) * 16) != (((CT)+15)&~15)
# error Wrong ScoringDecodedMessage member sizes.
#endif
    v16qi vector16[(CT+15)/16];
    text_t plain[(CT+15)&~15];
};

inline
void PrintDecodedMessage( FILE* stream, union ScoringDecodedMessage* message, int length ){
    int i = 0;
    for ( ; i< length; ++i ){
        fputc( message->plain[i] + 'A', stream );
    }
}

void DecodeScoredMessagePartStandard(const Key* const restrict key, int len, union ScoringDecodedMessage* restrict output);

bool GetDifferences( union ScoringDecodedMessage* restrict reference, union ScoringDecodedMessage* restrict tested, char* restrict output, int len );

#endif

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
