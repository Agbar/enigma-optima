#include <stdio.h>
#include <string.h>
#include <limits.h>

#ifndef WINDOWS
# include <unistd.h>
#endif

#include "cipher.h"
#include "ciphertext.h"
#include "date.h"
#include "error.h"
#include "global.h"
#include "key.h"
#include "result.h"
#include "config/array_sizes.h"
#include "config/types.h"

#ifndef PATH_MAX
# define PATH_MAX 255
#endif

FILE *open_outfile(char *s)
{
  FILE *fp = NULL;

  if ( (strlen(s) > PATH_MAX) || (fp = fopen(s, "a")) == NULL)
    err_open_fatal(s);

  return fp;
}

void print_plaintext(FILE *fp, const struct Key *const key, int len)
{
  int i;
  enigma_cipher_decoder_lookup.prepare_decoder_lookup_ALL(key, len);

  for (i = 0; i < len; i++) {
    struct echar c = decode(0,i,&key->stbrett);
    fputc( echar_to_alpha( c ), fp);
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

void print_key(FILE *fp, const struct Key *const key)
{
  char date[DATELEN];
  char stecker[27];
  int i;

  datestring(date);
  fprintf(fp, "Date: %s\n", date);

  for (i = 0; i < key->count; i++){
    stecker[i] = echar_to_ALPHA ( key->sf.map[i] );
  }
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
    UkwType_to_ALPHA( key->ukwnum ),
    key->slot.l.type, key->slot.m.type, key->slot.r.type,
    stecker,
    'A', echar_delta_to_ALPHA( key->ring.m ),
    echar_delta_to_ALPHA( key->ring.r ),
    echar_delta_to_ALPHA( key->mesg.l ), echar_delta_to_ALPHA( key->mesg.m ),
    echar_delta_to_ALPHA( key->mesg.r ));
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
    UkwType_to_ALPHA( key->ukwnum ),
    key->slot.g.type == GreekRingType_Beta ? 'B' : 'G', key->slot.l.type, key->slot.m.type, key->slot.r.type,
    stecker,
    'A', 'A',
    echar_delta_to_ALPHA( key->ring.m ), echar_delta_to_ALPHA( key->ring.r ),
    echar_delta_to_ALPHA( key->mesg.g ), echar_delta_to_ALPHA( key->mesg.l ),
    echar_delta_to_ALPHA( key->mesg.m ), echar_delta_to_ALPHA( key->mesg.r ));
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
