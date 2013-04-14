#ifndef CIPHER_H
#define CIPHER_H

#include <stdint.h>
#include <stdio.h>

#include "key.h"
#include "config\array_sizes.h"
#include "config\types.h"

int scrambler_state(const Key *key, int len);
void init_path_lookup_H_M3(const Key *key, int len);
void init_path_lookup_ALL(const Key *key, int len);
double dgetic_ALL(const Key *key, int len);
void en_deciph_stdin_ALL(FILE *file, const Key *key);


extern text_t ciphertext[];

// (&ciphertext[x])[i]; is a synonyme to: ciphertext[x+i];
// and is useful where ciphertext[x] can be calculated at compilation time.
#define DECODE(Cx,Offset,Index) \
    Cx = (&ciphertext[(Offset)])[(Index)]; \
    Cx = stbrett[(Cx)]; \
    Cx = path_lookup[Offset][(Index)*(LAST_DIMENSION)+(Cx)];\
    Cx = stbrett[(Cx)];

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
