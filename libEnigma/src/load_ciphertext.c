/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */

#include "ciphertext.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "config/types.h"
#include "error.h"
#include "global.h"


/* loads ciphertext into array, [A-Za-z] are converted to 0-25 */
void load_ciphertext(const char * const filename, int *const len, int resume)
{
  int c;
  FILE *fp;

  if ((fp = fopen(filename, "r")) == NULL) {
    if (resume)
      err_open_fatal_resume(filename);
    else
      err_open_fatal(filename);
  }

  // find number of characters in the message
  *len = 0;
  while ((c = fgetc(fp)) != EOF)
    if (isalpha(c))
      (*len)++;
    else if (!isspace(c))
      err_illegal_char_fatal(filename);

  rewind(fp);
  struct echar *p_ct = ciphertext.plain;
  while( ( c = fgetc( fp ) ) != EOF )
      if( isalpha( c ) ) {
          *p_ct = make_echar_ascii( c );
          ++p_ct;
      }

  fclose(fp);
}
