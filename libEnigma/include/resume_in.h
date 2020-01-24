#pragma once

#include <stdio.h>

#include "SwMode.h"
#include "key.h"

char *getline_resume(char *dest, int n, FILE *fp);
int set_state( struct Key* restrict from,
               struct Key* restrict to,
               struct Key* restrict ckey_res,
               struct Key* restrict gkey_res,
               struct SwMode* sw_mode,
               int* max_pass,
               int* firstpass );


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
