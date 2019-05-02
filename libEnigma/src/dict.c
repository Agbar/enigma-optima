#include <stdio.h>
#include <math.h>
#include "charmap.h"
#include "error.h"
#include "dict.h"
#include "config/types.h"
#include "config/array_sizes.h"
#include "echar/echar.h"

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

int load_tridict(const char *filename)
{
  unsigned char tri[4];
  int log;
  FILE *fp;

  if ( (fp = fopen(filename, "r")) == NULL )
    err_open_fatal(filename);

    while (fscanf(fp, "%3s%d", tri, &log) != EOF) {
        if (     !echar_can_make_from_ascii( tri[0] )
              || !echar_can_make_from_ascii( tri[1] )
              || !echar_can_make_from_ascii( tri[2] ) ){
            err_illegal_char_fatal(filename);
        }
        struct echar
            e0 = make_echar_ascii( tri[0] ),
            e1 = make_echar_ascii( tri[1] ),
            e2 = make_echar_ascii( tri[2] );
        tridict[echar_0_based_index( e0 )]
               [echar_0_based_index( e1 )]
               [echar_0_based_index( e2 )] = log;
    }

  fclose(fp);
  return 0;
}

int load_bidict(const char *filename)
{
  unsigned char bi[3];
  int log;
  FILE *fp;

  if ( (fp = fopen(filename, "r")) == NULL )
    err_open_fatal(filename);

    while (fscanf(fp, "%2s%d", bi, &log) != EOF) {
        if (     !echar_can_make_from_ascii( bi[0] )
              || !echar_can_make_from_ascii( bi[1] ) ){
            err_illegal_char_fatal(filename);
        }
        struct echar
            e0 = make_echar_ascii( bi[0] ),
            e1 = make_echar_ascii( bi[1] );
        bidict[echar_0_based_index( e0 )]
              [echar_0_based_index( e1 )] = log;
    }

  fclose(fp);
  return 0;
}


int load_unidict(const char *filename)
{
  unsigned char uni[2];
  int log;
  FILE *fp;

  if ( (fp = fopen(filename, "r")) == NULL )
    err_open_fatal(filename);

    while (fscanf(fp, "%1s%d", uni, &log) != EOF) {
        if ( !echar_can_make_from_ascii( uni[0] ) ){
            err_illegal_char_fatal(filename);
        }
        struct echar e = make_echar_ascii( uni[0] );
        unidict[echar_0_based_index( e )] = log;
    }

  fclose(fp);
  return 0;
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
