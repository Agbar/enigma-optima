#ifndef CIPHER_H
#define CIPHER_H

#include <stdint.h>
#include <stdio.h>

#include "ciphertext.h"
#include "cpu.h"
#include "global.h"
#include "key.h"
#include "config\array_sizes.h"
#include "config\types.h"

int scrambler_state(const Key *key, int len);
double dgetic_ALL(const Key *key, int len);
void en_deciph_stdin_ALL(FILE *file, const Key *key);

typedef void (*enigma_prepare_decoder_lookup_function_pt) (const Key *key, int len);

typedef struct _enigma_cipher_function_t
{
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup_M_H3;
    enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup_ALL;
} enigma_cipher_function_t;

void enigma_cipher_init(enigma_cpu_flags_t cpu, int machine_type, enigma_prepare_decoder_lookup_function_pt* cf);

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
#ifdef INLINE_IS_FAST
inline
text_t decode(size_t offset,size_t index, const PermutationMap_t* const stbrett)
{
    text_t c;
    c = (&ciphertext.plain[offset])[index];
    c = stbrett->letters[c];
    c = path_lookup[offset][index*LAST_DIMENSION+c];
    return stbrett->letters[c];
}
#else

#error DECODE macro needs refactoring
#define DECODE(Cx,Offset,Index) \
    Cx = (&ciphertext.plain[(Offset)])[(Index)]; \
    Cx = stbrett[(Cx)]; \
    Cx = path_lookup[Offset][(Index)*(LAST_DIMENSION)+(Cx)];\
    Cx = stbrett[(Cx)];

#endif

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
