#ifndef CIPHER_H
#define CIPHER_H

#include <stdint.h>
#include <stdio.h>

#include "ciphertext.h"
#include "cpu.h"
#include "global.h"
#include "key.h"
#include "ModMath.h"
#include "config/array_sizes.h"
#include "config/types.h"

struct Turnovers_t {
    int8_t r,
     r2,
     m,
     m2;
};

PURE_FUNCTION
int scrambler_state(const Key *key, int len);
PURE_FUNCTION
double dgetic_ALL(const Key *key, int len);

typedef void (*enigma_prepare_decoder_lookup_function_pt) (const Key *key, int len);

typedef struct _enigma_cipher_function_t
{
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup_M_H3;
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup_ALL;
} enigma_cipher_function_t;

void enigma_cipher_init(enigma_cpu_flags_t cpu, enum ModelType_t machine_type, enigma_prepare_decoder_lookup_function_pt* cf);

extern enigma_cipher_function_t enigma_cipher_decoder_lookup;
extern text_t path_lookup[CT][LAST_DIMENSION];

/*
 * decoders common data
 *************************/
extern text_t wal_turn[9];
extern text_t     wal[11][78] ;
extern text_t rev_wal[11][78];
extern text_t ukw[5][52];
extern text_t etw[52];

// (&ciphertext[x])[i]; is a synonyme to: ciphertext[x+i];
// and is useful where ciphertext[x] can be calculated at compilation time.
//
// path_lookup[Offset][(Index)*(LAST_DIMENSION)+(Cx)];
// is synonyme to
// path_lookup[Offset+Index][(Cx)];
static inline
text_t decode(size_t offset,size_t index, const PermutationMap_t* const stbrett)
{
    text_t c;
    c = (&ciphertext.plain[offset])[index];
    c = stbrett->letters[c];
    c = path_lookup[offset][index*LAST_DIMENSION+c];
    return stbrett->letters[c];
}

static inline
v2qs decode2( size_t offset, size_t index, const PermutationMap_t* const stbrett )
{
    text_t c;
    text_t d;
    c = (&ciphertext.plain[offset    ])[index];
    d = (&ciphertext.plain[offset + 1])[index];
    c = stbrett->letters[c];
    d = stbrett->letters[d];
    c = path_lookup[offset    ][index*LAST_DIMENSION+c];
    d = path_lookup[offset + 1][index*LAST_DIMENSION+d];

    v2qs ret = { stbrett->letters[c], stbrett->letters[d] };
    return ret;
}

static inline
v4qs decode4( size_t offset, size_t index, const PermutationMap_t* const stbrett )
{
    text_t c;
    text_t d;
    text_t e;
    text_t f;
    c = (&ciphertext.plain[offset    ])[index];
    d = (&ciphertext.plain[offset + 1])[index];
    e = (&ciphertext.plain[offset + 2])[index];
    f = (&ciphertext.plain[offset + 3])[index];
    c = stbrett->letters[c];
    d = stbrett->letters[d];
    e = stbrett->letters[e];
    f = stbrett->letters[f];
    c = path_lookup[offset    ][index*LAST_DIMENSION+c];
    d = path_lookup[offset + 1][index*LAST_DIMENSION+d];
    e = path_lookup[offset + 2][index*LAST_DIMENSION+e];
    f = path_lookup[offset + 3][index*LAST_DIMENSION+f];

    v4qs ret = { stbrett->letters[c]
               , stbrett->letters[d]
               , stbrett->letters[e]
               , stbrett->letters[f]
               };
    return ret;
}

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
