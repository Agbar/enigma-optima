#include <ctype.h>
#include <limits.h>


#include "config/types.h"


const text_t code[UCHAR_MAX+1];

void init_charmap(void)
{
  int i;
  text_t *init_code = (text_t*)code;

  for (i = 0; i < UCHAR_MAX+1; i++)
    init_code[i] = 26;

  for (i = 0; i < 26; i++) {
    init_code['a' + i] = i;
    init_code['A' + i] = i;
  }
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
