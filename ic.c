#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cipher.h"
#include "global.h"
#include "hillclimb.h"
#include "ic.h"
#include "key.h"
#include "config/types.h"


void ic_noring( const Key *from, const Key *to __attribute__((unused)), const Key *ckey_res, const Key *gkey_res,
                int sw_mode, int max_pass, int firstpass, int max_score, int resume,
                FILE *outfile, int act_on_sig, int len )
{
  Key ckey;
  Key gkey;
  Key lo;
  text_t hi[3][12] = {
    {EnigmaModel_H,  2,0,5,5,5,25,25, 0,25,25,25},
    {EnigmaModel_M3, 2,0,8,8,8,25,25, 0,25,25,25},
    {EnigmaModel_M4,4,10,8,8,8,25,25,25,25,25,25}
  };

  double a, bestic;
  int firstloop = 1, clen;

  enum ModelType_t m = from->model;

  /* iterate thru all but ring settings */
  ckey = gkey = lo = *from;
  ckey.ring.m = ckey.ring.r = 0;
  bestic = 0;
  for (ckey.ukwnum=lo.ukwnum; ckey.ukwnum<=hi[m][1]; ckey.ukwnum++) {
   for (ckey.slot.g=lo.slot.g; ckey.slot.g<=hi[m][2]; ckey.slot.g++) {
    for (ckey.slot.l=lo.slot.l; ckey.slot.l<=hi[m][3]; ckey.slot.l++) {
     for (ckey.slot.m=lo.slot.m; ckey.slot.m<=hi[m][4]; ckey.slot.m++) {
       if (ckey.slot.m == ckey.slot.l) continue;
      for (ckey.slot.r=lo.slot.r; ckey.slot.r<=hi[m][5]; ckey.slot.r++) {
        if (ckey.slot.r == ckey.slot.l || ckey.slot.r == ckey.slot.m) continue;
       for (ckey.mesg.g=lo.mesg.g; ckey.mesg.g<=hi[m][8]; ckey.mesg.g++) {
        for (ckey.mesg.l=lo.mesg.l; ckey.mesg.l<=hi[m][9]; ckey.mesg.l++) {
         for (ckey.mesg.m=lo.mesg.m; ckey.mesg.m<=hi[m][10]; ckey.mesg.m++) {
          for (ckey.mesg.r=lo.mesg.r; ckey.mesg.r<=hi[m][11]; ckey.mesg.r++) {

             a = dgetic_ALL(&ckey, len);
             if (a-bestic > DBL_EPSILON) {
               bestic = a;
               gkey = ckey;
             }

             if (firstloop) {
               firstloop = 0;
               init_key_low(&lo, m);
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

  /* recover ring settings */
  ckey = gkey;
  for (ckey.ring.r = 0; ckey.ring.r < 26; ckey.ring.r++) {
    a = dgetic_ALL(&ckey, len);
    if (a-bestic > DBL_EPSILON) {
      bestic = a;
      gkey = ckey;
    }
    ckey.mesg.r = (ckey.mesg.r+1) % 26;
  }
  ckey = gkey;
  for (ckey.ring.m = 0; ckey.ring.m < 26; ckey.ring.m++) {
    a = dgetic_ALL(&ckey, len);
    if (a-bestic > DBL_EPSILON) {
      bestic = a;
      gkey = ckey;
    }
    ckey.mesg.m = (ckey.mesg.m+1) % 26;
  }

  /* try to recover stecker of the best key (gkey) */
  clen = (len < CT) ? len : CT;
  hillclimb( &gkey, &gkey, ckey_res, gkey_res, sw_mode, max_pass, firstpass,
             max_score, resume, outfile, act_on_sig, clen );

}


void ic_allring( const Key *from, const Key *to, const Key *ckey_res, const Key *gkey_res,
                 int sw_mode, int max_pass, int firstpass, int max_score, int resume,
                 FILE *outfile, int act_on_sig, int len )
{
  Key ckey;
  Key gkey;
  Key lo;
  text_t hi[3][12] = {
    {EnigmaModel_H, 2,0,5,5,5,25,25,0,25,25,25},
    {EnigmaModel_M3,2,0,8,8,8,25,25,0,25,25,25},
    {EnigmaModel_M4,4,10,8,8,8,25,25,25,25,25,25}
  };
  double a, bestic;
  int firstloop = 1, clen;
  enum ModelType_t m = from->model;

  /* iterate thru all settings */
  ckey = gkey = lo = *from;
  bestic = 0;
  for (ckey.ukwnum=lo.ukwnum; ckey.ukwnum<=hi[m][1]; ckey.ukwnum++) {
   for (ckey.slot.g=lo.slot.g; ckey.slot.g<=hi[m][2]; ckey.slot.g++) {
    for (ckey.slot.l=lo.slot.l; ckey.slot.l<=hi[m][3]; ckey.slot.l++) {
     for (ckey.slot.m=lo.slot.m; ckey.slot.m<=hi[m][4]; ckey.slot.m++) {
       if (ckey.slot.m == ckey.slot.l) continue;
      for (ckey.slot.r=lo.slot.r; ckey.slot.r<=hi[m][5]; ckey.slot.r++) {
        if (ckey.slot.r == ckey.slot.l || ckey.slot.r == ckey.slot.m) continue;
       for (ckey.ring.m=lo.ring.m; ckey.ring.m<=hi[m][6]; ckey.ring.m++) {
        for (ckey.ring.r=lo.ring.r; ckey.ring.r<=hi[m][7]; ckey.ring.r++) {
         for (ckey.mesg.g=lo.mesg.g; ckey.mesg.g<=hi[m][8]; ckey.mesg.g++) {
          for (ckey.mesg.l=lo.mesg.l; ckey.mesg.l<=hi[m][9]; ckey.mesg.l++) {
           for (ckey.mesg.m=lo.mesg.m; ckey.mesg.m<=hi[m][10]; ckey.mesg.m++) {
            for (ckey.mesg.r=lo.mesg.r; ckey.mesg.r<=hi[m][11]; ckey.mesg.r++) {

             a = dgetic_ALL(&ckey, len);
             if (a-bestic > DBL_EPSILON) {
               bestic = a;
               gkey = ckey;
             }

             if (firstloop) {
               firstloop = 0;
               init_key_low(&lo, m);
             }
             if (keycmp(&ckey, to) == 0)
               goto HILLCLIMB;

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

  /* try to recover stecker of the best key (gkey) */
  HILLCLIMB:
  clen = (len < CT) ? len : CT;
  hillclimb( &gkey, &gkey, ckey_res, gkey_res, sw_mode, max_pass, firstpass,
             max_score, resume, outfile, act_on_sig, clen );

}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
