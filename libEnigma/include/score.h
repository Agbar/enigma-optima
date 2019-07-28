#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "cpu.h"
#include "global.h"
#include "key.h"
#include "cipher.h"

#include "common.h"

/** Best-fit type for length of part of the message used to compute scores.
 *  Size of scoreLength_t depends on value of CT macro. Selecting lowest possible
 *  allows compiler do better optimization choices ie. while unrolling.
 */
typedef uint8_t scoreLength_t;

STATIC_ASSERT ( ( scoreLength_t ) -1 > 0 , "scoreLength_t should be unsigned, so next assert can be correct." );
STATIC_ASSERT (  1 << ( sizeof( scoreLength_t ) * 8 ) > CT , "scoreLength_t is to narrow." );
STATIC_ASSERT ( UINT16_MAX > CT * CT, "uint16_t is to narrow for current CT value. Use ie. uint32_t." );

typedef struct _enigma_score_function_t{
    int      (*triscore) ( const struct Key* restrict key, scoreLength_t length );
    int      (* biscore) ( const struct Key* restrict key, scoreLength_t length );
    uint16_t (* icscore) ( const struct Key* restrict key, scoreLength_t length );
    int      (*uniscore) ( const struct Key* restrict key, scoreLength_t length );
} enigma_score_function_t;

// Flags are used to allow combining.
enum EnigmaScoreFunctions_t {
    EnigmaSF_Default    = 0, ///< Default means not set.
    EnigmaSF_Original   = 1,
    EnigmaSF_Simple     = 2,
    EnigmaSF_Optimized  = 4,
    EnigmaSF_SSSE3      = 8,
    EnigmaSF_Avx        = 1 << 4,
    EnigmaSF_Avx2       = 1 << 5,
    // for experimental versions go from top
    EnigmaSF_OptNoInterleave = 1 << 31,
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


#define TRISCORE_TAIL (2)

union ScoringDecodedMessage
{
    union v16_echar vector16[(CT+TRISCORE_TAIL+15)/16];
    union v32_echar vector32[(CT+TRISCORE_TAIL+31)/32];
    struct echar plain[(CT+TRISCORE_TAIL+15)&~15];
};

void DecodeScoredMessagePartStandard(const struct Key* restrict key, int len, union ScoringDecodedMessage* restrict output);

bool GetDifferences( union ScoringDecodedMessage* restrict reference, union ScoringDecodedMessage* restrict tested, char* restrict output, int len );

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
