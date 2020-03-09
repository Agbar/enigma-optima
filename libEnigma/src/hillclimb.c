
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "character_encoding.h"
#include "config/types.h"
#include "global.h"
#include "hillclimb.h"
#include "iterators/mesg_iterator.h"
#include "iterators/ringstellung_iterator.h"
#include "iterators/slot_iterator.h"
#include "key.h"
#include "score.h"
#include "state.h"
#include "stecker.h"


static const struct Key hi_h = {
    .model = EnigmaModel_H,
    .ukwnum = {2},
    .slot = {.l = {5}, .m = {5}, .r = {5}},
    .ring = {.m = {25}, .r = {25}},
    .mesg = {.l = {25}, .m = {25}, .r = {25}},
};

static const struct Key hi_m3 = {
    .model = EnigmaModel_M3,
    .ukwnum = {2},
    .slot = {.l = {8}, .m = {8}, .r = {8}},
    .ring = {.m = {25}, .r = {25}},
    .mesg = {.l = {25}, .m = {25}, .r = {25}},
};

static const struct Key hi_m4 = {
    .model = EnigmaModel_M4,
    .ukwnum = {4},
    .slot = {.g = {10}, .l = {8}, .m = {8}, .r = {8}},
    .ring = {.m = {25}, .r = {25}},
    .mesg = {.g = {25}, .l = {25}, .m = {25}, .r = {25}},
};

PURE_FUNCTION
const struct Key* get_hi_key( enum ModelType_t model );


void hillclimb( struct State* state,
                int max_pass,
                int len,
                const struct HillclimbersKnapsack* knapsack )
{
    if( !check_knapsack( knapsack ) ) exit( 911 );

  struct echar var[26];
  Fill0To25_echar(var);
  bool firstloop = 1;

    knapsack->log( "enigma: working on range ..." );

    struct Key* ckey = state->ckey;
    struct Key* gkey = state->gkey;
    int* pass = &state->pass;
    struct Key lo = *state->ckey;
    const struct Key* const restrict hi = get_hi_key( state->from->model );
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
   *ckey = lo;
   firstloop = 1;

   for (ckey->ukwnum=lo.ukwnum; ckey->ukwnum.type<=hi->ukwnum.type; ckey->ukwnum.type++) {

       struct SlotIterator slot_iter = init_SlotIterator( &ckey->slot, ckey->model );
       for( ; !SlotIterator_equ( slot_overflow(), slot_iter );
            slot_iter.next( &slot_iter ) ) {

        struct RingstellungIterator ring_iter = {.state = &ckey->ring, .m = ckey->slot.m, .r = ckey->slot.r};
        for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
             next_ringstellung( &ring_iter ) ) {

            struct MesgIterator mesg_iter = {.state = &ckey->mesg};
            for( ; !MesgIterator_equ( mesg_overflow(), mesg_iter );
                 mesg_iter = next_mesg( mesg_iter, state->from->model ) ) {
                knapsack->check_shutdown( state );

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
                 init_key_low( &lo, state->from->model );
               }
               if( Key_equ( ckey, state->to ) ) {
                   goto RESTART;
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
    if( !knapsack->check_shutdown ) return false;
    if( !knapsack->log ) return false;
    if( !knapsack->scrambler_state_is_endloop ) return false;
    return true;
}


static bool endloop_check_sw_onstart( const struct Key* ckey, int len ) {
    return scrambler_state( ckey, len ).mode != SW_ONSTART;
}


static bool endloop_check_sw_other( const struct Key* ckey, int len ) {
    return scrambler_state( ckey, len ).mode != SW_OTHER;
}


static bool endloop_check_sw_all( const struct Key* ckey, int len ) {
    return scrambler_state( ckey, len ).mode == SW_NONE;
}


static bool endloop_check_always_false( UNUSED const struct Key* ckey, UNUSED int len ) {
    return false;
}


scrambler_state_is_endloop_f* select_scrambler_state_is_endloop_impl( const struct State* state ) {
    switch( state->sw_mode.mode ) {
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


const struct Key* get_hi_key( enum ModelType_t model ) {
    switch( model ) {
    case EnigmaModel_H:
        return &hi_h;
    case EnigmaModel_M3:
        return &hi_m3;
    case EnigmaModel_M4:
        return &hi_m4;
    default:
        UNREACHABLE();
    }
}

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
