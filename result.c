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
#include "config\array_sizes.h"
#include "config\types.h"

FILE *open_outfile(char *s)
{
  FILE *fp = NULL;

  if ( (strlen(s) > PATH_MAX) || (fp = fopen(s, "a")) == NULL)
    err_open_fatal(s);

  return fp;
}

void print_plaintext(FILE *fp, const PermutationMap_t *stbrett, int len)
{
  int i;
  text_t c;
  int ofd;

  for (i = 0; i < len; i++) {
    c = decode(0,i,stbrett);
    fputc(alpha[c], fp);
  }
  fputc('\n', fp);
  fputc('\n', fp);
  fputc('\n', fp);

  fflush(fp);
#ifndef WINDOWS
  ofd = fileno(fp);
  fsync(ofd);
#endif
}

void print_key(FILE *fp, const Key *key)
{
  char date[DATELEN];
  char stecker[27];
  int i;
  int ofd;

  datestring(date);
  fprintf(fp, "Date: %s\n", date);

  for (i = 0; i < key->count; i++)
    stecker[i] = toupper(alpha[key->sf[i]]);
  stecker[i] = '\0';

  if (key->model != M4) {
    fprintf(fp,
"Score: %d\n\
UKW: %c\n\
W/0: %d%d%d\n\
Stecker: %s\n\
Rings: %c%c%c\n\
Message key: %c%c%c\n\n",
    key->score,
    toupper(alpha[key->ukwnum]),
    key->l_slot, key->m_slot, key->r_slot,
    stecker,
    toupper(alpha[key->l_ring]), toupper(alpha[key->m_ring]),
    toupper(alpha[key->r_ring]),
    toupper(alpha[key->l_mesg]), toupper(alpha[key->m_mesg]),
    toupper(alpha[key->r_mesg]));
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
    key->g_slot == 9 ? 'B' : 'G', key->l_slot, key->m_slot, key->r_slot,
    stecker,
    toupper(alpha[key->g_ring]), toupper(alpha[key->l_ring]),
    toupper(alpha[key->m_ring]), toupper(alpha[key->r_ring]),
    toupper(alpha[key->g_mesg]), toupper(alpha[key->l_mesg]),
    toupper(alpha[key->m_mesg]), toupper(alpha[key->r_mesg]));
  }

  fflush(fp);
#ifndef WINDOWS
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
