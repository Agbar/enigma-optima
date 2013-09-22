#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "cipher.h"
#include "ciphertext.h"
#include "dict.h"
#include "error.h"
#include "global.h"
#include "hillclimb.h"
#include "key.h"
#include "result.h"
#include "resume_out.h"
#include "score.h"
#include "stecker.h"
#include "state.h"
#include "config\array_sizes.h"
#include "config\testing.h"
#include "config\types.h"
#include "OS\Os.h"


void save_state(State state)
{
  FILE *fp;

  if ((fp = fopen("00hc.resume", "w")) == NULL)
    err_open_fatal("00hc.resume");

  print_state(fp, &state);
  if (ferror(fp) != 0)
    err_stream_fatal("00hc.resume");

  fclose(fp);
}

void save_state_exit(State state, int retval)
{
  FILE *fp;

  if ((fp = fopen("00hc.resume", "w")) == NULL)
    err_open_fatal("00hc.resume");

  print_state(fp, &state);
  if (ferror(fp) != 0)
    err_stream_fatal("00hc.resume");

  exit(retval);
}

void hillclimb( const Key *from, const Key *to, const Key *ckey_res, const Key *gkey_res,
                int sw_mode, int max_pass, int firstpass, int max_score, int resume,
                FILE *outfile, int act_on_sig, int len )
{
  Key ckey;
  Key gkey;
  Key lo;
  text_t hi[3][12] = {
    {EnigmaModel_H ,2, 0,5,5,5,25,25, 0,25,25,25},
    {EnigmaModel_M3,2, 0,8,8,8,25,25, 0,25,25,25},
    {EnigmaModel_M4,4,10,8,8,8,25,25,25,25,25,25}
  };
  Change ch;
  State state;
  time_t lastsave;
  int m;
  int i, k, x, z;
  text_t var[26] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
  int pass, newtop, action;
  int bestscore, jbestscore, a, globalscore;
  double bestic, ic;
  int firstloop = 1;

  enigma_score_function_t sf;

  enigma_score_init(enigma_cpu_flags, &sf);

  enigma_prepare_decoder_lookup_function_pt prepare_decoder_lookup;

  enigma_cipher_init(enigma_cpu_flags, from->model, &prepare_decoder_lookup);

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
    for (ckey.slot.g=lo.slot.g; ckey.slot.g<=hi[m][2]; ckey.slot.g++) {
     for (ckey.slot.l=lo.slot.l; ckey.slot.l<=hi[m][3]; ckey.slot.l++) {
      for (ckey.slot.m=lo.slot.m; ckey.slot.m<=hi[m][4]; ckey.slot.m++) {
        if (ckey.slot.m == ckey.slot.l) continue;
       for (ckey.slot.r=lo.slot.r; ckey.slot.r<=hi[m][5]; ckey.slot.r++) {
         if (ckey.slot.r == ckey.slot.l || ckey.slot.r == ckey.slot.m) continue;
        for (ckey.ring.m=lo.ring.m; ckey.ring.m<=hi[m][6]; ckey.ring.m++) {
          if (ckey.slot.m > 5 && ckey.ring.m > 12) continue;
         for (ckey.ring.r=lo.ring.r; ckey.ring.r<=hi[m][7]; ckey.ring.r++) {
           if (ckey.slot.r > 5 && ckey.ring.r > 12) continue;
          for (ckey.mesg.g=lo.mesg.g; ckey.mesg.g<=hi[m][8]; ckey.mesg.g++) {
           for (ckey.mesg.l=lo.mesg.l; ckey.mesg.l<=hi[m][9]; ckey.mesg.l++) {
            for (ckey.mesg.m=lo.mesg.m; ckey.mesg.m<=hi[m][10]; ckey.mesg.m++) {
             for (ckey.mesg.r=lo.mesg.r; ckey.mesg.r<=hi[m][11]; ckey.mesg.r++) {

               if (doShutdown)
                 save_state_exit(state, 111);
               if (difftime(time(NULL), lastsave) > 119) {
                 lastsave = time(NULL);
                 save_state(state);
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
               Fill0To25(ckey.sf);
               Fill0To25(ckey.stbrett.letters);
               ckey.count = 0;

               /* initialize path_lookup */
               prepare_decoder_lookup(&ckey, len);


               /* ic score */
               bestic = sf.icscore(&ckey, len);
               for (i = 0; i < 26; i++) {
                 for (k = i+1; k < 26; k++) {
                   if ( (var[i] == ckey.stbrett.letters[var[i]] && var[k] == ckey.stbrett.letters[var[k]])
                      ||(var[i] == ckey.stbrett.letters[var[k]] && var[k] == ckey.stbrett.letters[var[i]]) ) {
                     swap(&ckey.stbrett, var[i], var[k]);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       continue;
                     }
                     swap(&ckey.stbrett, var[i], var[k]);
                   }
                   else if (var[i] == ckey.stbrett.letters[var[i]] && var[k] != ckey.stbrett.letters[var[k]]) {
                     action = NONE;
                     z = ckey.stbrett.letters[var[k]];
                     swap(&ckey.stbrett, var[k], z);

                     swap(&ckey.stbrett, var[i], var[k]);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = KZ_IK;
                     }
                     swap(&ckey.stbrett, var[i], var[k]);

                     swap(&ckey.stbrett, var[i], z);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = KZ_IZ;
                     }
                     swap(&ckey.stbrett, var[i], z);

                     switch (action) {
                       case KZ_IK:
                         swap(&ckey.stbrett, var[i], var[k]);
                         break;
                       case KZ_IZ:
                         swap(&ckey.stbrett, var[i], z);
                         break;
                       case NONE:
                         swap(&ckey.stbrett, var[k], z);
                         break;
                       default:
                         break;
                     }
                   }
                   else if (var[k] == ckey.stbrett.letters[var[k]] && var[i] != ckey.stbrett.letters[var[i]]) {
                     action = NONE;
                     x = ckey.stbrett.letters[var[i]];
                     swap(&ckey.stbrett, var[i], x);

                     swap(&ckey.stbrett, var[k], var[i]);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = IX_KI;
                     }
                     swap(&ckey.stbrett, var[k], var[i]);

                     swap(&ckey.stbrett, var[k], x);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = IX_KX;
                     }
                     swap(&ckey.stbrett, var[k], x);

                     switch (action) {
                       case IX_KI:
                         swap(&ckey.stbrett, var[k], var[i]);
                         break;
                       case IX_KX:
                         swap(&ckey.stbrett, var[k], x);
                         break;
                       case NONE:
                         swap(&ckey.stbrett, var[i], x);
                         break;
                       default:
                         break;
                     }
                   }
                   else if (var[i] != ckey.stbrett.letters[var[i]] && var[k] != ckey.stbrett.letters[var[k]]) {
                     action = NONE;
                     x = ckey.stbrett.letters[var[i]];
                     z = ckey.stbrett.letters[var[k]];
                     swap(&ckey.stbrett, var[i], x);
                     swap(&ckey.stbrett, var[k], z);

                     swap(&ckey.stbrett, var[i], var[k]);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = IXKZ_IK;
                     }
                     swap(&ckey.stbrett, x, z);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = IXKZ_IKXZ;
                     }
                     swap(&ckey.stbrett, x, z);
                     swap(&ckey.stbrett, var[i], var[k]);

                     swap(&ckey.stbrett, var[i], z);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = IXKZ_IZ;
                     }
                     swap(&ckey.stbrett, x, var[k]);
                     ic = sf.icscore(&ckey, len);
                     if (ic-bestic > DBL_EPSILON) {
                       bestic = ic;
                       action = IXKZ_IZXK;
                     }
                     swap(&ckey.stbrett, x, var[k]);
                     swap(&ckey.stbrett, var[i], z);

                     switch (action) {
                       case IXKZ_IK:
                         swap(&ckey.stbrett, var[i], var[k]);
                         break;
                       case IXKZ_IZ:
                         swap(&ckey.stbrett, var[i], z);
                         break;
                       case IXKZ_IKXZ:
                         swap(&ckey.stbrett, var[i], var[k]);
                         swap(&ckey.stbrett, x, z);
                       break;
                       case IXKZ_IZXK:
                         swap(&ckey.stbrett, var[i], z);
                         swap(&ckey.stbrett, x, var[k]);
                       break;
                       case NONE:
                         swap(&ckey.stbrett, var[i], x);
                         swap(&ckey.stbrett, var[k], z);
                         break;
                       default:
                         break;
                     }
                   }
                 }
               }


               newtop = 1;
               jbestscore = sf.triscore(&ckey, len) + sf.biscore(&ckey, len);

               while (newtop) {

                 newtop = 0;

                 bestscore = sf.biscore(&ckey, len);
                 for (i = 0; i < 26; i++) {
                   for (k = i+1; k < 26; k++) {
                     if ( (var[i] == ckey.stbrett.letters[var[i]] && var[k] == ckey.stbrett.letters[var[k]])
                        ||(var[i] == ckey.stbrett.letters[var[k]] && var[k] == ckey.stbrett.letters[var[i]]) ) {
                       swap(&ckey.stbrett, var[i], var[k]);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         continue;
                       }
                       swap(&ckey.stbrett, var[i], var[k]);
                     }
                     else if (var[i] == ckey.stbrett.letters[var[i]] && var[k] != ckey.stbrett.letters[var[k]]) {
                       action = NONE;
                       z = ckey.stbrett.letters[var[k]];
                       swap(&ckey.stbrett, var[k], z);

                       swap(&ckey.stbrett, var[i], var[k]);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = KZ_IK;
                       }
                       swap(&ckey.stbrett, var[i], var[k]);

                       swap(&ckey.stbrett, var[i], z);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = KZ_IZ;
                       }
                       swap(&ckey.stbrett, var[i], z);

                       switch (action) {
                         case KZ_IK:
                           swap(&ckey.stbrett, var[i], var[k]);
                           break;
                         case KZ_IZ:
                           swap(&ckey.stbrett, var[i], z);
                           break;
                         case NONE:
                           swap(&ckey.stbrett, var[k], z);
                           break;
                         default:
                           break;
                       }
                     }
                     else if (var[k] == ckey.stbrett.letters[var[k]] && var[i] != ckey.stbrett.letters[var[i]]) {
                       action = NONE;
                       x = ckey.stbrett.letters[var[i]];
                       swap(&ckey.stbrett, var[i], x);

                       swap(&ckey.stbrett, var[k], var[i]);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IX_KI;
                       }
                       swap(&ckey.stbrett, var[k], var[i]);

                       swap(&ckey.stbrett, var[k], x);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IX_KX;
                       }
                       swap(&ckey.stbrett, var[k], x);

                       switch (action) {
                         case IX_KI:
                           swap(&ckey.stbrett, var[k], var[i]);
                           break;
                         case IX_KX:
                           swap(&ckey.stbrett, var[k], x);
                           break;
                         case NONE:
                           swap(&ckey.stbrett, var[i], x);
                           break;
                         default:
                           break;
                       }
                     }
                     else if (var[i] != ckey.stbrett.letters[var[i]] && var[k] != ckey.stbrett.letters[var[k]]) {
                       action = NONE;
                       x = ckey.stbrett.letters[var[i]];
                       z = ckey.stbrett.letters[var[k]];
                       swap(&ckey.stbrett, var[i], x);
                       swap(&ckey.stbrett, var[k], z);

                       swap(&ckey.stbrett, var[i], var[k]);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IK;
                       }
                       swap(&ckey.stbrett, x, z);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IKXZ;
                       }
                       swap(&ckey.stbrett, x, z);
                       swap(&ckey.stbrett, var[i], var[k]);

                       swap(&ckey.stbrett, var[i], z);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IZ;
                       }
                       swap(&ckey.stbrett, x, var[k]);
                       a = sf.biscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IZXK;
                       }
                       swap(&ckey.stbrett, x, var[k]);
                       swap(&ckey.stbrett, var[i], z);

                       switch (action) {
                         case IXKZ_IK:
                           swap(&ckey.stbrett, var[i], var[k]);
                           break;
                         case IXKZ_IZ:
                           swap(&ckey.stbrett, var[i], z);
                           break;
                         case IXKZ_IKXZ:
                           swap(&ckey.stbrett, var[i], var[k]);
                           swap(&ckey.stbrett, x, z);
                           break;
                         case IXKZ_IZXK:
                           swap(&ckey.stbrett, var[i], z);
                           swap(&ckey.stbrett, x, var[k]);
                           break;
                         case NONE:
                           swap(&ckey.stbrett, var[i], x);
                           swap(&ckey.stbrett, var[k], z);
                           break;
                         default:
                           break;
                       }
                     }
                   }
                 }


                 bestscore = sf.triscore(&ckey, len);
                 for (i = 0; i < 26; i++) {
                   for (k = i+1; k < 26; k++) {
                     if ( (var[i] == ckey.stbrett.letters[var[i]] && var[k] == ckey.stbrett.letters[var[k]])
                        ||(var[i] == ckey.stbrett.letters[var[k]] && var[k] == ckey.stbrett.letters[var[i]]) ) {
                       swap(&ckey.stbrett, var[i], var[k]);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         continue;
                       }
                       swap(&ckey.stbrett, var[i], var[k]);
                     }
                     else if (var[i] == ckey.stbrett.letters[var[i]] && var[k] != ckey.stbrett.letters[var[k]]) {
                       action = NONE;
                       z = ckey.stbrett.letters[var[k]];
                       swap(&ckey.stbrett, var[k], z);

                       swap(&ckey.stbrett, var[i], var[k]);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = KZ_IK;
                       }
                       swap(&ckey.stbrett, var[i], var[k]);

                       swap(&ckey.stbrett, var[i], z);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = KZ_IZ;
                       }
                       swap(&ckey.stbrett, var[i], z);

                       switch (action) {
                         case KZ_IK:
                           swap(&ckey.stbrett, var[i], var[k]);
                           break;
                         case KZ_IZ:
                           swap(&ckey.stbrett, var[i], z);
                           break;
                         case NONE:
                           swap(&ckey.stbrett, var[k], z);
                           break;
                         default:
                           break;
                       }
                     }
                     else if (var[k] == ckey.stbrett.letters[var[k]] && var[i] != ckey.stbrett.letters[var[i]]) {
                       action = NONE;
                       x = ckey.stbrett.letters[var[i]];
                       swap(&ckey.stbrett, var[i], x);

                       swap(&ckey.stbrett, var[k], var[i]);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IX_KI;
                       }
                       swap(&ckey.stbrett, var[k], var[i]);

                       swap(&ckey.stbrett, var[k], x);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IX_KX;
                       }
                       swap(&ckey.stbrett, var[k], x);

                       switch (action) {
                         case IX_KI:
                           swap(&ckey.stbrett, var[k], var[i]);
                           break;
                         case IX_KX:
                           swap(&ckey.stbrett, var[k], x);
                           break;
                         case NONE:
                           swap(&ckey.stbrett, var[i], x);
                           break;
                         default:
                           break;
                       }
                     }
                     else if (var[i] != ckey.stbrett.letters[var[i]] && var[k] != ckey.stbrett.letters[var[k]]) {
                       action = NONE;
                       x = ckey.stbrett.letters[var[i]];
                       z = ckey.stbrett.letters[var[k]];
                       swap(&ckey.stbrett, var[i], x);
                       swap(&ckey.stbrett, var[k], z);

                       swap(&ckey.stbrett, var[i], var[k]);
                       a = sf.triscore(&ckey,  len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IK;
                       }
                       swap(&ckey.stbrett, x, z);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IKXZ;
                       }
                       swap(&ckey.stbrett, x, z);
                       swap(&ckey.stbrett, var[i], var[k]);

                       swap(&ckey.stbrett, var[i], z);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IZ;
                       }
                       swap(&ckey.stbrett, x, var[k]);
                       a = sf.triscore(&ckey, len);
                       if (a > bestscore) {
                         bestscore = a;
                         action = IXKZ_IZXK;
                       }
                       swap(&ckey.stbrett, x, var[k]);
                       swap(&ckey.stbrett, var[i], z);

                       switch (action) {
                         case IXKZ_IK:
                           swap(&ckey.stbrett, var[i], var[k]);
                           break;
                         case IXKZ_IZ:
                           swap(&ckey.stbrett, var[i], z);
                           break;
                         case IXKZ_IKXZ:
                           swap(&ckey.stbrett, var[i], var[k]);
                           swap(&ckey.stbrett, x, z);
                           break;
                         case IXKZ_IZXK:
                           swap(&ckey.stbrett, var[i], z);
                           swap(&ckey.stbrett, x, var[k]);
                           break;
                         case NONE:
                           swap(&ckey.stbrett, var[i], x);
                           swap(&ckey.stbrett, var[k], z);
                           break;
                         default:
                           break;
                       }
                     }
                   }
                 }


                 a = sf.triscore(&ckey, len) + sf.biscore(&ckey, len);
                 if (a > jbestscore) {
                   jbestscore = a;
                   newtop = 1;
                 }

               }


               get_stecker(&ckey);
               bestscore = sf.triscore(&ckey, len);

               newtop = 1;

               while (newtop) {

                 newtop = 0;
                 action = NONE;

                 /* try reswapping each self-steckered with each pair,
                  * steepest ascent */
                 for (i = 0; i < ckey.count; i += 2) {
                   swap(&ckey.stbrett, ckey.sf[i], ckey.sf[i+1]);
                   for (k = ckey.count; k < 26; k++) {
                     swap(&ckey.stbrett, ckey.sf[i], ckey.sf[k]);
                     a = sf.triscore(&ckey, len);
                     if (a > bestscore) {
                       newtop = 1;
                       action = RESWAP;
                       bestscore = a;
                       ch.u1 = i;
                       ch.u2 = i+1;
                       ch.s1 = k;
                       ch.s2 = i;
                     }
                     swap(&ckey.stbrett, ckey.sf[i], ckey.sf[k]);
                     swap(&ckey.stbrett, ckey.sf[i+1], ckey.sf[k]);
                     a = sf.triscore(&ckey, len);
                     if (a > bestscore) {
                       newtop = 1;
                       action = RESWAP;
                       bestscore = a;
                       ch.u1 = i;
                       ch.u2 = i+1;
                       ch.s1 = k;
                       ch.s2 = i+1;
                     }
                     swap(&ckey.stbrett, ckey.sf[i+1], ckey.sf[k]);
                   }
                   swap(&ckey.stbrett, ckey.sf[i], ckey.sf[i+1]);
                 }
                 if (action == RESWAP) {
                   swap(&ckey.stbrett, ckey.sf[ch.u1], ckey.sf[ch.u2]);
                   swap(&ckey.stbrett, ckey.sf[ch.s1], ckey.sf[ch.s2]);
                   get_stecker(&ckey);
                 }
                 action = NONE;

               }


               /* record global max, if applicable */
               if (bestscore > globalscore) {
                 globalscore = bestscore;
                 gkey = ckey;
                 gkey.score = bestscore;
                 print_key(outfile, &gkey);
                 print_plaintext(outfile, &gkey.stbrett, len);
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
    save_state_exit(state, EXIT_SUCCESS);

}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
