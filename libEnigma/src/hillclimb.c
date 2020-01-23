
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "OS/Os.h"
#include "character_encoding.h"
#include "config/types.h"
#include "global.h"
#include "hillclimb.h"
#include "key.h"
#include "score.h"
#include "state.h"
#include "stecker.h"


void hillclimb( struct State* state,
                int max_pass,
                int len,
                const struct HillclimbersKnapsack* knapsack )
{
    if( !check_knapsack( knapsack ) ) exit( 911 );

  text_t hi[3][12] = {
    {EnigmaModel_H ,2, 0,5,5,5,25,25, 0,25,25,25},
    {EnigmaModel_M3,2, 0,8,8,8,25,25, 0,25,25,25},
    {EnigmaModel_M4,4,10,8,8,8,25,25,25,25,25,25}
  };

  struct echar var[26];
  Fill0To25_echar(var);
  int firstloop = 1;

    knapsack->log( "enigma: working on range ..." );

    struct Key* ckey = state->ckey;
    struct Key* gkey = state->gkey;
    int* pass = &state->pass;
    int m = state->from->model;
    struct Key lo = *state->ckey;
    uint32_t globalscore = gkey->score;

  if( state->firstpass )
    /* set testing order to letter frequency in ciphertext */
    set_to_ct_freq(var, len);
  else
    /* set random testing order */
    rand_var(var);


  /* with the rand_var() restarting method passes are independent
   * from each other
   */
  for( *pass = max_pass; *pass > 0; --*pass, lo = *state->from ) {

   firstloop = 1;

   for (ckey->ukwnum=lo.ukwnum; ckey->ukwnum.type<=hi[m][1]; ckey->ukwnum.type++) {
    for (ckey->slot.g=lo.slot.g; ckey->slot.g.type<=hi[m][2]; ckey->slot.g.type++) {
     for (ckey->slot.l=lo.slot.l; ckey->slot.l.type<=hi[m][3]; ckey->slot.l.type++) {
      for (ckey->slot.m=lo.slot.m; ckey->slot.m.type<=hi[m][4]; ckey->slot.m.type++) {
        if (ckey->slot.m.type == ckey->slot.l.type) continue;
       for (ckey->slot.r=lo.slot.r; ckey->slot.r.type<=hi[m][5]; ckey->slot.r.type++) {
         if (ckey->slot.r.type == ckey->slot.l.type || ckey->slot.r.type == ckey->slot.m.type) continue;
        for (ckey->ring.m=lo.ring.m; ckey->ring.m.delta<=hi[m][6]; ckey->ring.m.delta++) {
          if (ckey->slot.m.type > 5 && ckey->ring.m.delta > 12) continue;
         for (ckey->ring.r=lo.ring.r; ckey->ring.r.delta<=hi[m][7]; ckey->ring.r.delta++) {
           if (ckey->slot.r.type > 5 && ckey->ring.r.delta > 12) continue;
          for (ckey->mesg.g=lo.mesg.g; ckey->mesg.g.delta<=hi[m][8]; ckey->mesg.g.delta++) {
           for (ckey->mesg.l=lo.mesg.l; ckey->mesg.l.delta<=hi[m][9]; ckey->mesg.l.delta++) {
            for (ckey->mesg.m=lo.mesg.m; ckey->mesg.m.delta<=hi[m][10]; ckey->mesg.m.delta++) {
             for (ckey->mesg.r=lo.mesg.r; ckey->mesg.r.delta<=hi[m][11]; ckey->mesg.r.delta++) {

                if( doShutdown ) {
                    knapsack->save_state( state, true );
                    exit( 111 );
                }
                else {
                    knapsack->save_state( state, false );
                }

               /* avoid duplicate scrambler states */
                if( knapsack->scrambler_state_is_endloop( ckey, len ) ) {
                    goto ENDLOOP;
                }

               /* complete ckey initialization */
               Fill0To25_echar( ckey->sf.map );
               Fill0To25_echar( ckey->stbrett.letters );
               ckey->count = 0;

               struct ScoreOptimizer* opt = knapsack->optimizer;
               /* initialize path_lookup */
               opt->prepare_decoder_lookup( ckey, len );
               uint32_t bestscore = opt->optimize_score( var, ckey, len, opt->score_impl );

               /* record global max, if applicable */
               if ( bestscore > globalscore ) {
                 globalscore = bestscore;
                 *gkey = *ckey;
                 gkey->score = bestscore;
                 knapsack->on_new_best( gkey, len );
               }

               ENDLOOP:
               if (firstloop) {
                 firstloop = 0;
                 init_key_low(&lo, m);
               }
               if( keycmp( ckey, state->to ) == 0 )
                 goto RESTART;

             }
            }
           }
          }
         }
        }
       }
      }
     }
    }
   }

   /* choose random order of testing stecker */
   RESTART:
   state->firstpass = false;
   rand_var(var);

  }

    knapsack->log( "enigma: finished range" );
    knapsack->save_state( state, true );
}


bool check_knapsack( const struct HillclimbersKnapsack* knapsack ) {
    if( !knapsack->optimizer ) return false;
    if( !knapsack->on_new_best ) return false;
    if( !knapsack->save_state ) return false;
    if( !knapsack->log ) return false;
    if( !knapsack->scrambler_state_is_endloop ) return false;
    return true;
}


static bool endloop_check_sw_onstart( const struct Key* ckey, int len ) {
    return scrambler_state( ckey, len ) != SW_ONSTART;
}


static bool endloop_check_sw_other( const struct Key* ckey, int len ) {
    return scrambler_state( ckey, len ) != SW_OTHER;
}


static bool endloop_check_sw_all( const struct Key* ckey, int len ) {
    return scrambler_state( ckey, len ) == SW_NONE;
}


static bool endloop_check_always_false( UNUSED const struct Key* ckey, UNUSED int len ) {
    return false;
}


scrambler_state_is_endloop_f* select_scrambler_state_is_endloop_impl( const struct State* state ) {
    switch( state->sw_mode ) {
    case SW_ONSTART:
        return &endloop_check_sw_onstart;
    case SW_OTHER:
        return &endloop_check_sw_other;
    case SW_ALL:
        return &endloop_check_sw_all;
    case SINGLE_KEY:
    default:
        return &endloop_check_always_false;
    }
}

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
