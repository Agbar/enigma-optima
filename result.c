#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include "cipher.h"
#include "ciphertext.h"
#include "charmap.h"
#include "date.h"
#include "error.h"
#include "global.h"
#include "key.h"
#include "result.h"
#include "config/array_sizes.h"
#include "config/types.h"

FILE *open_outfile(char *s)
{
  FILE *fp = NULL;

  if ( (strlen(s) > PATH_MAX) || (fp = fopen(s, "a")) == NULL)
    err_open_fatal(s);

  return fp;
}

void print_plaintext(FILE *fp, const Key *const key, int len)
{
  int i;
  text_t c;
  enigma_cipher_decoder_lookup.prepare_decoder_lookup_ALL(key, len);

  for (i = 0; i < len; i++) {
    c = decode(0,i,&key->stbrett);
    fputc(alpha[c], fp);
  }
  fputc('\n', fp);
  fputc('\n', fp);
  fputc('\n', fp);

  fflush(fp);
#ifndef WINDOWS
  int ofd;
  ofd = fileno(fp);
  fsync(ofd);
#endif
}

void print_key(FILE *fp, const Key *key)
{
  char date[DATELEN];
  char stecker[27];
  int i;

  datestring(date);
  fprintf(fp, "Date: %s\n", date);

  for (i = 0; i < key->count; i++)
    stecker[i] = toupper(alpha[key->sf[i]]);
  stecker[i] = '\0';

  if (key->model != EnigmaModel_M4) {
    fprintf(fp,
"Score: %d\n\
UKW: %c\n\
W/0: %d%d%d\n\
Stecker: %s\n\
Rings: %c%c%c\n\
Message key: %c%c%c\n\n",
    key->score,
    toupper(alpha[key->ukwnum]),
    key->slot.l, key->slot.m, key->slot.r,
    stecker,
    toupper(alpha[key->ring.l]), toupper(alpha[key->ring.m]),
    toupper(alpha[key->ring.r]),
    toupper(alpha[key->mesg.l]), toupper(alpha[key->mesg.m]),
    toupper(alpha[key->mesg.r]));
  }
  else {
    fprintf(fp,
"Score: %d\n\
UKW: %c\n\
W/0: %c%d%d%d\n\
Stecker: %s\n\
Rings: %c%c%c%c\n\
Message key: %c%c%c%c\n\n",
    key->score,
    key->ukwnum == 3 ? 'B' : 'C',
    key->slot.g == 9 ? 'B' : 'G', key->slot.l, key->slot.m, key->slot.r,
    stecker,
    toupper(alpha[key->ring.g]), toupper(alpha[key->ring.l]),
    toupper(alpha[key->ring.m]), toupper(alpha[key->ring.r]),
    toupper(alpha[key->mesg.g]), toupper(alpha[key->mesg.l]),
    toupper(alpha[key->mesg.m]), toupper(alpha[key->mesg.r]));
  }

  fflush(fp);
#ifndef WINDOWS
  int ofd;
  ofd = fileno(fp);
  fsync(ofd);
#endif
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
