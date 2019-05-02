#ifndef CIPHERTEXT_H
#define CIPHERTEXT_H

#include <stdint.h>
#include "config/types.h"
#include "character_encoding.h"

void load_ciphertext(const char *filename, int *len, int resume);

union ciphertext_t
{ 
    struct echar plain[2048];
    union v16_echar vector16[128];
    union v32_echar vector32[64];
};

extern union ciphertext_t ciphertext;

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
