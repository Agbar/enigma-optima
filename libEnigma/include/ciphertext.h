#ifndef CIPHERTEXT_H
#define CIPHERTEXT_H

#include <stdint.h>
#include "config/types.h"

void load_ciphertext(const char *filename, int *len, int resume);

typedef union _Ciphertext
{
    text_t plain[2048];
    v16qi  vector16[128];
    v32qi  vector32[64];
} ciphertext_t;

extern ciphertext_t ciphertext;

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
