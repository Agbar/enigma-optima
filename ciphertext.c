#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "charmap.h"
#include "error.h"
#include "ciphertext.h"


/* loads ciphertext into array, [A-Za-z] are converted to 0-25 */
uint8_t * load_ciphertext(const char * const filename, int *const len, int resume)
{
  int c;
  uint8_t *ciphertext, *p_ct;
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

  if (( ciphertext = malloc(*len*(sizeof *ciphertext)) ) == NULL)
    err_alloc_fatal(filename);

  rewind(fp);
  p_ct = ciphertext;
  while ((c = fgetc(fp)) != EOF)
    if (isalpha(c))
      *p_ct++ = code[c];

  fclose(fp);
  return ciphertext;

}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
