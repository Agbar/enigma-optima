#ifndef DICT_H
#define DICT_H

#include "config\types.h"
#include "config\array_sizes.h"


int load_tridict(const char *filename);
int load_bidict(const char *filename);
int load_unidict(const char *filename);

extern dict_t tridict[26][LAST_DIMENSION][LAST_DIMENSION];
extern dict_t bidict[26][LAST_DIMENSION];
extern dict_t unidict[26];

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
