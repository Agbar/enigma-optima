#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "charmap.h"
#include "global.h"
#include "key.h"
#include "state.h"
#include "resume_out.h"


void print_state(FILE *fp, const State *state)
{
  char stecker[27];
  int i;
  int ofd;

  const Key *from = state->from;
  const Key *to = state->to;
  Key *ckey = state->ckey;
  Key *gkey = state->gkey;
  int *sw_mode = state->sw_mode;
  int *pass = state->pass;
  int *firstpass = state->firstpass;
  int *max_score = state->max_score;


  /* general state */
  if (from->model == H) fprintf(fp, "H=");
  else if (from->model == M3) fprintf(fp, "M3=");
  else if (from->model== M4) fprintf(fp, "M4=");

  if (from->model != M4) {
    fprintf(fp,
    "%c:%d%d%d:%c%c:%c%c%c=%c:%d%d%d:%c%c:%c%c%c=%c:%d%d%d:%c%c:%c%c%c=",
    toupper(alpha[from->ukwnum]),
    from->slot.l, from->slot.m, from->slot.r,
    toupper(alpha[from->ring.m]), toupper(alpha[from->ring.r]),
    toupper(alpha[from->mesg.l]), toupper(alpha[from->mesg.m]),
    toupper(alpha[from->mesg.r]),
    toupper(alpha[to->ukwnum]),
    to->slot.l, to->slot.m, to->slot.r,
    toupper(alpha[to->ring.m]), toupper(alpha[to->ring.r]),
    toupper(alpha[to->mesg.l]), toupper(alpha[to->mesg.m]),
    toupper(alpha[to->mesg.r]),
    toupper(alpha[ckey->ukwnum]),
    ckey->slot.l, ckey->slot.m, ckey->slot.r,
    toupper(alpha[ckey->ring.m]), toupper(alpha[ckey->ring.r]),
    toupper(alpha[ckey->mesg.l]), toupper(alpha[ckey->mesg.m]),
    toupper(alpha[ckey->mesg.r]));
  }
  else {
    fprintf(fp,
    "%c:%c%d%d%d:%c%c:%c%c%c%c=%c:%c%d%d%d:%c%c:%c%c%c%c=%c:%c%d%d%d:%c%c:%c%c%c%c=",
    from->ukwnum == 3 ? 'B' : 'C',
    from->slot.g == 9 ? 'B' : 'G', from->slot.l, from->slot.m, from->slot.r,
    toupper(alpha[from->ring.m]), toupper(alpha[from->ring.r]),
    toupper(alpha[from->mesg.g]), toupper(alpha[from->mesg.l]),
    toupper(alpha[from->mesg.m]), toupper(alpha[from->mesg.r]),
    to->ukwnum == 3 ? 'B' : 'C',
    to->slot.g == 9 ? 'B' : 'G', to->slot.l, to->slot.m, to->slot.r,
    toupper(alpha[to->ring.m]), toupper(alpha[to->ring.r]),
    toupper(alpha[to->mesg.g]), toupper(alpha[to->mesg.l]),
    toupper(alpha[to->mesg.m]), toupper(alpha[to->mesg.r]),
    ckey->ukwnum == 3 ? 'B' : 'C',
    ckey->slot.g == 9 ? 'B' : 'G', ckey->slot.l, ckey->slot.m, ckey->slot.r,
    toupper(alpha[ckey->ring.m]), toupper(alpha[ckey->ring.r]),
    toupper(alpha[ckey->mesg.g]), toupper(alpha[ckey->mesg.l]),
    toupper(alpha[ckey->mesg.m]), toupper(alpha[ckey->mesg.r]));
  }

  fprintf(fp, "%d=", *sw_mode);
  fprintf(fp, "%d=", *pass);
  fprintf(fp, "%d=", *firstpass);
  fprintf(fp, "%d\n", *max_score);


  /* global key */
  if (from->model == H) fprintf(fp, "H=");
  else if (from->model == M3) fprintf(fp, "M3=");
  else if (from->model== M4) fprintf(fp, "M4=");

  if (from->model != M4) {
    fprintf(fp,
    "%c:%d%d%d:%c%c:%c%c%c=",
    toupper(alpha[gkey->ukwnum]),
    gkey->slot.l, gkey->slot.m, gkey->slot.r,
    toupper(alpha[gkey->ring.m]), toupper(alpha[gkey->ring.r]),
    toupper(alpha[gkey->mesg.l]), toupper(alpha[gkey->mesg.m]),
    toupper(alpha[gkey->mesg.r]));
  }
  else {
    fprintf(fp,
    "%c:%c%d%d%d:%c%c:%c%c%c%c=",
    gkey->ukwnum == 3 ? 'B' : 'C',
    gkey->slot.g == 9 ? 'B' : 'G', gkey->slot.l, gkey->slot.m, gkey->slot.r,
    toupper(alpha[gkey->ring.m]), toupper(alpha[gkey->ring.r]),
    toupper(alpha[gkey->mesg.g]), toupper(alpha[gkey->mesg.l]),
    toupper(alpha[gkey->mesg.m]), toupper(alpha[gkey->mesg.r]));
  }

  for (i = 0; i < gkey->count; i++)
    stecker[i] = toupper(alpha[gkey->sf[i]]);
  stecker[i] = '\0';
  fprintf(fp, "%s=", stecker);

  fprintf(fp, "%d\n", gkey->score);


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
