#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "charmap.h"
#include "error.h"
#include "ciphertext.h"
#include "config\types.h"


// Limit imposed by highest value in dictionary 71964 (bidict.naval) and size
// of scoring variable (32-bit signed) gives limit of ciphertext
// length of about 29800 (very pessimistically).
// I assume that 1024 characters should be enough in any realistic scenario,
// especially that message is trimmed to 250 characters (CT in global.h).
text_t ciphertext[1024];


/* loads ciphertext into array, [A-Za-z] are converted to 0-25 */
void load_ciphertext(const char * const filename, int *const len, int resume)
{
  int c;
  text_t *p_ct;
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
  p_ct = ciphertext;
  while ((c = fgetc(fp)) != EOF)
    if (isalpha(c))
      *p_ct++ = code[c];

  fclose(fp);
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
