#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "character_encoding.h"
#include "cipher.h"
#include "ciphertext.h"
#include "dict.h"
#include "error.h"
#include "global.h"
#include "hillclimb2.h"
#include "key.h"
#include "randomNumbers.h"
#include "result.h"
#include "resume_out.h"
#include "score.h"
#include "stecker.h"
#include "state.h"

#include "OS/Os.h"

void save_state2(State state)
{
  FILE *fp;

  if ((fp = fopen("00hc.resume", "w")) == NULL)
    err_open_fatal("00hc.resume");

  print_state(fp, &state);
  if (ferror(fp) != 0)
    err_stream_fatal("00hc.resume");

  fclose(fp);
}

void save_state_exit2(State state, int retval)
{
  FILE *fp;

  if ((fp = fopen("00hc.resume", "w")) == NULL)
    err_open_fatal("00hc.resume");

  print_state(fp, &state);
  if (ferror(fp) != 0)
    err_stream_fatal("00hc.resume");

  exit(retval);
}

void hillclimb2( const struct Key* const from, const struct Key* const to, const struct Key* const ckey_res, const struct Key* const gkey_res,
                int sw_mode, int max_pass, int firstpass, int max_score, int resume,
                FILE *outfile, int act_on_sig, int len )
{
  struct Key ckey;
  struct Key gkey;
  struct Key lo;
  text_t hi[3][12] = {
    {EnigmaModel_H ,2, 0,5,5,5,25,25, 0,25,25,25},
    {EnigmaModel_M3,2, 0,8,8,8,25,25, 0,25,25,25},
    {EnigmaModel_M4,4,10,8,8,8,25,25,25,25,25,25}
  };
  State state;
  time_t lastsave;
  int m;
  int p, q;
  struct echar i, k, x, z, u, v;
  struct echar var[26];
  Fill0To25_echar(var);
  int pass;
  int bestscore, jbestscore, a, globalscore;
  uint16_t bestic, ic;
  int firstloop = 1;

  enigma_score_function_t sf;

  enigma_score_init( enigma_cpu_flags, &sf );

  enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup;

  enigma_cipher_init( enigma_cpu_flags, from->model, &prepare_decoder_lookup );

  SetupRandomGenerator();

  lastsave = time(NULL);

  if (resume) {
    hillclimb_log("enigma: working on range ...");
  }

  if (act_on_sig) {
    state.from = from;
    state.to = to;
    state.ckey = &ckey;
    state.gkey = &gkey;
    state.sw_mode = &sw_mode;
    state.pass = &pass;
    state.firstpass = &firstpass;
    state.max_score = &max_score;
    state.ciphertext = ciphertext.plain;
  
    InstallSighandler();
  }

  m = from->model;
  ckey = lo = (resume) ? *ckey_res : *from;
  gkey = (resume) ? *gkey_res : *from;
  globalscore = (resume) ? gkey_res->score : 0;


  if (firstpass)
    /* set testing order to letter frequency in ciphertext */
    set_to_ct_freq(var, len);
  else
    /* set random testing order */
    rand_var(var);


  /* with the rand_var() restarting method passes are independent
   * from each other
   */
  for (pass = max_pass; pass > 0; pass--, lo = *from) {

   firstloop = 1;

   for (ckey.ukwnum=lo.ukwnum; ckey.ukwnum<=hi[m][1]; ckey.ukwnum++) {
    for (ckey.slot.g=lo.slot.g; ckey.slot.g.type<=hi[m][2]; ckey.slot.g.type++) {
     for (ckey.slot.l=lo.slot.l; ckey.slot.l.type<=hi[m][3]; ckey.slot.l.type++) {
      for (ckey.slot.m=lo.slot.m; ckey.slot.m.type<=hi[m][4]; ckey.slot.m.type++) {
        if (ckey.slot.m.type == ckey.slot.l.type) continue;
       for (ckey.slot.r=lo.slot.r; ckey.slot.r.type<=hi[m][5]; ckey.slot.r.type++) {
         if (ckey.slot.r.type == ckey.slot.l.type || ckey.slot.r.type == ckey.slot.m.type) continue;
        for (ckey.ring.m=lo.ring.m; ckey.ring.m.delta<=hi[m][6]; ckey.ring.m.delta++) {
          if (ckey.slot.m.type > 5 && ckey.ring.m.delta > 12) continue;
         for (ckey.ring.r=lo.ring.r; ckey.ring.r.delta<=hi[m][7]; ckey.ring.r.delta++) {
           if (ckey.slot.r.type > 5 && ckey.ring.r.delta > 12) continue;
          for (ckey.mesg.g=lo.mesg.g; ckey.mesg.g.delta<=hi[m][8]; ckey.mesg.g.delta++) {
           for (ckey.mesg.l=lo.mesg.l; ckey.mesg.l.delta<=hi[m][9]; ckey.mesg.l.delta++) {
            for (ckey.mesg.m=lo.mesg.m; ckey.mesg.m.delta<=hi[m][10]; ckey.mesg.m.delta++) {
             for (ckey.mesg.r=lo.mesg.r; ckey.mesg.r.delta<=hi[m][11]; ckey.mesg.r.delta++) {

               if (doShutdown)
                 save_state_exit2(state, 111);
               if (difftime(time(NULL), lastsave) > 119) {
                 lastsave = time(NULL);
                 save_state2(state);
               }


               /* avoid duplicate scrambler states */
               switch (sw_mode) {
                 case SW_ONSTART:
                   if (scrambler_state(&ckey, len) != SW_ONSTART)
                     goto ENDLOOP;
                   break;
                 case SW_OTHER:
                   if (scrambler_state(&ckey, len) != SW_OTHER)
                     goto ENDLOOP;
                   break;
                 case SW_ALL:
                   if (scrambler_state(&ckey, len) == SW_NONE)
                     goto ENDLOOP;
                   break;
                 default: /* includes SINGLE_KEY */
                   break;
               } 

               /* complete ckey initialization */
               Fill0To25_echar( ckey.sf.map );
               Fill0To25_echar( ckey.stbrett.letters );
               ckey.count = 0;

               /* initialize path_lookup */
               prepare_decoder_lookup( &ckey, len );

               /* ic score */
               bestic = sf.icscore( &ckey, len );
			   for (p = 0; p < 25; p++) {
				   for (q = p + 1; q < 26; q++) {
					   i = var[p];
					   k = var[q];
					   x = ckey.stbrett.letters[ echar_0_based_index( i ) ];
					   z = ckey.stbrett.letters[ echar_0_based_index( k ) ];
					   u = ckey.stbrett.letters[ echar_0_based_index( x ) ];
					   v = ckey.stbrett.letters[ echar_0_based_index( z ) ];

					   if ( echar_eq( x, k ) ){
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = i;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = k;
					   }
					   else {
						   if ( echar_neq( x, i ) ){
							   ckey.stbrett.letters[ echar_0_based_index( i ) ] = i;
							   ckey.stbrett.letters[ echar_0_based_index( x ) ] = x;
						   };
						   if ( echar_neq( z, k ) ){
							   ckey.stbrett.letters[ echar_0_based_index( k ) ] = k;
							   ckey.stbrett.letters[ echar_0_based_index( z ) ] = z;
						   };
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = k;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = i;
					   }

					   ic = sf.icscore( &ckey, len );

					   if (ic > bestic) {
						   bestic = ic;
					   }
					   else {
						   ckey.stbrett.letters[ echar_0_based_index( z ) ] = v;
						   ckey.stbrett.letters[ echar_0_based_index( x ) ] = u;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = z;
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = x;
					   }

				   }
			   }

			   bestscore = sf.uniscore( &ckey, len );
			   for (p = 0; p < 25; p++) {
				   for (q = p + 1; q < 26; q++) {
					   i = var[p];
					   k = var[q];
					   x = ckey.stbrett.letters[ echar_0_based_index( i ) ];
					   z = ckey.stbrett.letters[ echar_0_based_index( k ) ];
					   u = ckey.stbrett.letters[ echar_0_based_index( x ) ];
					   v = ckey.stbrett.letters[ echar_0_based_index( z ) ];

					   if ( echar_eq( x, k ) ){
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = i;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = k;
					   }
					   else {
						   if ( echar_neq( x, i ) ){
							   ckey.stbrett.letters[ echar_0_based_index( i ) ] = i;
							   ckey.stbrett.letters[ echar_0_based_index( x ) ] = x;
						   };
						   if ( echar_neq( z, k ) ){
							   ckey.stbrett.letters[ echar_0_based_index( k ) ] = k;
							   ckey.stbrett.letters[ echar_0_based_index( z ) ] = z;
						   };
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = k;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = i;
					   }

					   a = sf.uniscore( &ckey, len );

					   if (a > bestscore) {
						   bestscore = a;
					   }
					   else {
						   ckey.stbrett.letters[ echar_0_based_index( z ) ] = v;
						   ckey.stbrett.letters[ echar_0_based_index( x ) ] = u;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = z;
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = x;
					   }

				   }
			   }


			   bestscore = sf.triscore( &ckey, len );
               do {
				 jbestscore = bestscore;
                 for (p = 0; p < 25; p++) {
                   for (q = p+1; q < 26; q++) {
					   i = var[p];
					   k = var[q];
					   x = ckey.stbrett.letters[ echar_0_based_index( i ) ];
					   z = ckey.stbrett.letters[ echar_0_based_index( k ) ];
					   u = ckey.stbrett.letters[ echar_0_based_index( x ) ];
					   v = ckey.stbrett.letters[ echar_0_based_index( z ) ];

					   if ( echar_eq( x, k ) ){
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = i;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = k;
					   }
					   else {
						   if ( echar_neq( x, i ) ){
							   ckey.stbrett.letters[ echar_0_based_index( i ) ] = i;
							   ckey.stbrett.letters[ echar_0_based_index( x ) ] = x;
						   };
						   if ( echar_neq( z, k ) ){
							   ckey.stbrett.letters[ echar_0_based_index( k ) ] = k;
							   ckey.stbrett.letters[ echar_0_based_index( z ) ] = z;
						   };
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = k;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = i;
					   }

                       a = sf.triscore( &ckey, len );

                       if (a > bestscore) {                 
						   bestscore = a;
                       }
					   else {
						   ckey.stbrett.letters[ echar_0_based_index( z ) ] = v;
						   ckey.stbrett.letters[ echar_0_based_index( x ) ] = u;
						   ckey.stbrett.letters[ echar_0_based_index( k ) ] = z;
						   ckey.stbrett.letters[ echar_0_based_index( i ) ] = x;
					   }
                     
                   }
                 }

			   } while (bestscore > jbestscore);


               /* record global max, if applicable */
			   get_stecker(&ckey);
			   if (bestscore > globalscore) {
                 globalscore = bestscore;
                 gkey = ckey;
                 gkey.score = bestscore;
                 print_key(outfile, &gkey);
                 print_plaintext( outfile, &gkey, len );
                 if (ferror(outfile) != 0) {
                   fputs("enigma: error: writing to result file failed\n", stderr);
                   exit(EXIT_FAILURE);
                 }
               }
               /* abort if max_score is reached */
               if (globalscore > max_score)
                 goto FINISHED;
                

               ENDLOOP:
               if (firstloop) {
                 firstloop = 0;
                 init_key_low(&lo, m);
               }
               if (keycmp(&ckey, to) == 0)
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
   firstpass = 0;
   rand_var(var);

  }

  FINISHED:
  if (resume)
    hillclimb_log("enigma: finished range");
  if (act_on_sig)
    save_state_exit2(state, EXIT_SUCCESS);

}


/*
 * This file is part of enigma-suite, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2016 Alex Shovkoplyas (VE3NEA)
 *
 */
