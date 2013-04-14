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
double dgetic_ALL(const Key *key, const text_t *ciphertext, int len);
void en_deciph_stdin_ALL(FILE *file, const Key *key);

#define DECODE(Cx,Offset,Index) \
    Cx = ciphertext[(Offset)+(Index)]; \
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
