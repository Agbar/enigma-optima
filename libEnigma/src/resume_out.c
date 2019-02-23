#include <stdio.h>
#include <ctype.h>

#ifndef WINDOWS
# include <unistd.h>
#endif

#include "charmap.h"
#include "global.h"
#include "key.h"
#include "state.h"
#include "resume_out.h"


void print_state(FILE *fp, const State *state)
{
  char stecker[27];
  int i;

  const struct Key *from = state->from;
  const struct Key *to = state->to;
  struct Key *ckey = state->ckey;
  struct Key *gkey = state->gkey;
  int *sw_mode = state->sw_mode;
  int *pass = state->pass;
  int *firstpass = state->firstpass;
  int *max_score = state->max_score;


  /* general state */
  if (from->model == EnigmaModel_H) fprintf(fp, "H=");
  else if (from->model == EnigmaModel_M3) fprintf(fp, "M3=");
  else if (from->model == EnigmaModel_M4) fprintf(fp, "M4=");

  if (from->model != EnigmaModel_M4) {
    fprintf(fp,
    "%c:%d%d%d:%c%c:%c%c%c=%c:%d%d%d:%c%c:%c%c%c=%c:%d%d%d:%c%c:%c%c%c=",
    toupper(alpha[from->ukwnum]),
    from->slot.l.type, from->slot.m.type, from->slot.r.type,
    echar_delta_to_ALPHA( from->ring.m ), echar_delta_to_ALPHA( from->ring.r ),
    echar_delta_to_ALPHA( from->mesg.l ), echar_delta_to_ALPHA( from->mesg.m ),
    echar_delta_to_ALPHA( from->mesg.r ),
    toupper(alpha[to->ukwnum]),
    to->slot.l.type, to->slot.m.type, to->slot.r.type,
    echar_delta_to_ALPHA( to->ring.m ), echar_delta_to_ALPHA( to->ring.r ),
    echar_delta_to_ALPHA( to->mesg.l ), echar_delta_to_ALPHA( to->mesg.m ),
    echar_delta_to_ALPHA( to->mesg.r ),
    toupper(alpha[ckey->ukwnum]),
    ckey->slot.l.type, ckey->slot.m.type, ckey->slot.r.type,
    echar_delta_to_ALPHA( ckey->ring.m ), echar_delta_to_ALPHA( ckey->ring.r ),
    echar_delta_to_ALPHA( ckey->mesg.l ), echar_delta_to_ALPHA( ckey->mesg.m ),
    echar_delta_to_ALPHA( ckey->mesg.r ));
  }
  else {
    fprintf(fp,
    "%c:%c%d%d%d:%c%c:%c%c%c%c=%c:%c%d%d%d:%c%c:%c%c%c%c=%c:%c%d%d%d:%c%c:%c%c%c%c=",
    // from
    from->ukwnum == 3 ? 'B' : 'C',
    //:
    from->slot.g.type == GreekRingType_Beta ? 'B' : 'G', from->slot.l.type, from->slot.m.type, from->slot.r.type,
    //:
    echar_delta_to_ALPHA( from->ring.m ), echar_delta_to_ALPHA( from->ring.r ) ,
    //:
    echar_delta_to_ALPHA( from->mesg.g ), echar_delta_to_ALPHA( from->mesg.l ) ,
    echar_delta_to_ALPHA( from->mesg.m ), echar_delta_to_ALPHA( from->mesg.r ) ,
    //= // to
    to->ukwnum == 3 ? 'B' : 'C',
    //:
    to->slot.g.type == GreekRingType_Beta ? 'B' : 'G', to->slot.l.type, to->slot.m.type, to->slot.r.type,
    //:
    echar_delta_to_ALPHA( to->ring.m ), echar_delta_to_ALPHA( to->ring.r ),
    //:
    echar_delta_to_ALPHA( to->mesg.g ), echar_delta_to_ALPHA( to->mesg.l ),
    echar_delta_to_ALPHA( to->mesg.m ), echar_delta_to_ALPHA( to->mesg.r ),
    //= // current
    ckey->ukwnum == 3 ? 'B' : 'C',
    //:
    ckey->slot.g.type == GreekRingType_Beta ? 'B' : 'G', ckey->slot.l.type, ckey->slot.m.type, ckey->slot.r.type,
    //:
    echar_delta_to_ALPHA( ckey->ring.m ), echar_delta_to_ALPHA( ckey->ring.r ),
    //:
    echar_delta_to_ALPHA( ckey->mesg.g ), echar_delta_to_ALPHA( ckey->mesg.l ),
    echar_delta_to_ALPHA( ckey->mesg.m ), echar_delta_to_ALPHA( ckey->mesg.r ));
  }

  fprintf(fp, "%d=", *sw_mode);
  fprintf(fp, "%d=", *pass);
  fprintf(fp, "%d=", *firstpass);
  fprintf(fp, "%d\n", *max_score);


  /* global key */
  if (from->model == EnigmaModel_H) fprintf(fp, "H=");
  else if (from->model == EnigmaModel_M3) fprintf(fp, "M3=");
  else if (from->model== EnigmaModel_M4) fprintf(fp, "M4=");

  if (from->model != EnigmaModel_M4) {
    fprintf(fp,
    "%c:%d%d%d:%c%c:%c%c%c=",
    toupper(alpha[gkey->ukwnum]),
    gkey->slot.l.type, gkey->slot.m.type, gkey->slot.r.type,
    echar_delta_to_ALPHA( gkey->ring.m ), echar_delta_to_ALPHA( gkey->ring.r ),
    echar_delta_to_ALPHA( gkey->mesg.l ), echar_delta_to_ALPHA( gkey->mesg.m ),
    echar_delta_to_ALPHA( gkey->mesg.r ));
  }
  else {
    fprintf(fp,
    "%c:%c%d%d%d:%c%c:%c%c%c%c=",
    gkey->ukwnum == 3 ? 'B' : 'C',
    gkey->slot.g.type == GreekRingType_Beta ? 'B' : 'G', gkey->slot.l.type, gkey->slot.m.type, gkey->slot.r.type,
    echar_delta_to_ALPHA( gkey->ring.m ), echar_delta_to_ALPHA( gkey->ring.r ),
    echar_delta_to_ALPHA( gkey->mesg.g ), echar_delta_to_ALPHA( gkey->mesg.l ),
    echar_delta_to_ALPHA( gkey->mesg.m ), echar_delta_to_ALPHA( gkey->mesg.r ));
  }

  for (i = 0; i < gkey->count; i++)
    stecker[i] = toupper(alpha[ echar_0_based_index( gkey->sf.map[i] ) ]);
  stecker[i] = '\0';
  fprintf(fp, "%s=", stecker);

  fprintf(fp, "%d\n", gkey->score);


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
