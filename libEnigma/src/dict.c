
#include "dict.h"
#include "config/types.h"
#include "config/array_sizes.h"

// This is quite bad.
// dict  | samples |  sat  |  sat
//       |         | 26624 | 17576
// --------------------------------
// naval |    2957 |   11% |   15%
// u534  |    2530 |   10% |   14%
// gen   |    1576 |    7% |    9%
//
// sizeof: 106496 (int)

dict_t tridict[26][LAST_DIMENSION][LAST_DIMENSION];

// Much better here:
// dict  | samples | sat 832 | sat 676
// -----------------------------------
// naval |     524 |     63% |     78%
// u534  |     559 |     67% |     83%
// gen   |     376 |     45% |     56%
//
// sizeof: 3328
dict_t bidict[26][LAST_DIMENSION];

// not used.
dict_t unidict[26];


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
