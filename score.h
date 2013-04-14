#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

#include "key.h"

int get_triscore(const Key *key, int len);
double icscore(const text_t *stbrett, int len);
int uniscore(const text_t *stbrett,  int len);
int biscore(const text_t *stbrett, int len);
int triscore(const text_t *stbrett, int len);

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
