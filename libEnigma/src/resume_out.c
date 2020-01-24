#include <stdio.h>

#ifndef WINDOWS
# include <unistd.h>
#endif

#include "global.h"
#include "key.h"
#include "state.h"
#include "resume_out.h"

static
void print_key_rings( const struct Key * key, char buffer[16] );

void print_state(FILE *fp, const struct State *state)
{
  char stecker[27];
  int i;

  const struct Key *from = state->from;
  const struct Key *to = state->to;
  struct Key *ckey = state->ckey;
  struct Key *gkey = state->gkey;

  /* general state */
  if (from->model == EnigmaModel_H) fprintf(fp, "H=");
  else if (from->model == EnigmaModel_M3) fprintf(fp, "M3=");
  else if (from->model == EnigmaModel_M4) fprintf(fp, "M4=");

    char from_buffer[16];
    char to_buffer[16];
    char ckey_buffer[16];
    print_key_rings( from, from_buffer );
    print_key_rings( to, to_buffer );
    print_key_rings( ckey, ckey_buffer );
    fprintf( fp, "%s%s%s", from_buffer, to_buffer, ckey_buffer );

    fprintf( fp, "%d=", state->sw_mode.mode );
    fprintf( fp, "%d=", state->pass );
    fprintf( fp, "%d=", state->firstpass );
    fprintf( fp, "%d\n", MAX_SCORE );


  /* global key */
  if (from->model == EnigmaModel_H) fprintf(fp, "H=");
  else if (from->model == EnigmaModel_M3) fprintf(fp, "M3=");
  else if (from->model== EnigmaModel_M4) fprintf(fp, "M4=");

    char gkey_buffer[16];
    print_key_rings( gkey, gkey_buffer );
    fprintf( fp, "%s", gkey_buffer );

  for (i = 0; i < gkey->count; i++){
    stecker[i] =  echar_to_ALPHA( gkey->sf.map[i] );
  }
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

static
void print_key_rings( const struct Key * key, char buffer[16] ){
    size_t buflen = 16;
    if( key->model  != EnigmaModel_M4 ){
        snprintf ( buffer, buflen,
          "%c:%c%c%c:%c%c:%c%c%c=",
          UkwType_to_ALPHA( key->ukwnum ),
          // :
          RingType_to_ALPHA( key->slot.l ), RingType_to_ALPHA( key->slot.m ), RingType_to_ALPHA( key->slot.r),
          // :
          echar_delta_to_ALPHA( key->ring.m ), echar_delta_to_ALPHA( key->ring.r ),
          // :
          echar_delta_to_ALPHA( key->mesg.l ), echar_delta_to_ALPHA( key->mesg.m ),
          echar_delta_to_ALPHA( key->mesg.r ) );
    }
    else {
      snprintf ( buffer, buflen,
        "%c:%c%c%c%c:%c%c:%c%c%c%c=",
        UkwType_to_ALPHA( key->ukwnum ),
        //:
        GreekRingType_to_ALPHA( key->slot.g ), RingType_to_ALPHA( key->slot.l ), RingType_to_ALPHA( key->slot.m ), RingType_to_ALPHA( key->slot.r ),
        //:
        echar_delta_to_ALPHA( key->ring.m ), echar_delta_to_ALPHA( key->ring.r ) ,
        //:
        echar_delta_to_ALPHA( key->mesg.g ), echar_delta_to_ALPHA( key->mesg.l ) ,
        echar_delta_to_ALPHA( key->mesg.m ), echar_delta_to_ALPHA( key->mesg.r ) );
    }
}



/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
