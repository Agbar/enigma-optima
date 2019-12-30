#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "banner.h"
#include "cipher.h"
#include "ciphertext.h"
#include "cpu.h"
#include "display.h"
#include "error.h"
#include "global.h"
#include "input.h"
#include "key.h"
#include "optimizer.h"
#include "randomNumbers.h"
#include "result.h"
#include "resume_in.h"
#include "resume_out.h"
#include "scan.h"
#include "config/types.h"
#include "OS/Os.h"


void SetupOsThingsAndStuff( void ) {

    // Linked to one of OS\os* modules
    SetupProcessProperties();

}

int main(int argc, char **argv)
{
  struct Key key;
  struct Key from, to, ckey_res, gkey_res;
  int len, clen;
  enum ModelType_t model = EnigmaModel_H;
  int opt;
  bool first = true;
  bool optimizerOptionPresent = false;
  int hc = 0;
  int sw_mode = SW_ONSTART;
  int max_pass = 1, firstpass = 1;
  int max_score = INT_MAX-1, resume = 0;
  FILE *outfile = stdout;
  char *f = NULL, *t = NULL;
  char *fmin[3] = {
    "A:123:AA:AAA",
    "B:123:AA:AAA",
    "B:B123:AA:AAAA"
  };
  char *tmax[3] = {
    "C:543:ZZ:ZZZ",
    "C:876:MM:ZZZ",
    "C:G876:MM:ZZZZ"
  };

  enigma_cpu_flags_init(enigma_cpu_all);

  WriteStartupBanner();

  init_key_default(&key, model);

  const struct option longOpts[] = {
  { .name = "optimizer", .has_arg = required_argument,  .val = 0x101 }
    , {0}
  };

  int option_index = 0;
  opterr = 0;
  while (( opt = getopt_long(argc, argv, "hvcRM:f:t:o:", longOpts, &option_index )) != -1) {
    switch (opt) {
      case 'h': help(); break;
      case 'v': version(); break;
      // deprecated
      case 'c': hc = 1; break;
      // deprecated
      case 'f': f = optarg; break;
      // deprecated
      case 't': t = optarg; break;
      // E@H
      case 'R': resume = 1; hc = 1; break;
      // E@H
      case 'o': if (!(outfile = open_outfile(optarg))) usage(); break;
      // deprecated
      case 'M': if ((model = get_model(optarg)) == EnigmaModel_Error || !first) usage();
                if (!init_key_default(&key, model)) usage(); break;
      case 0x101:
            if ( optimizerOptionPresent || !selectOptimizer( optarg ) ) {
                usage();
            }
            optimizerOptionPresent = true;
            break;
      default: usage();
    }
    first = false;
  }

  if (hc == 0) {
    usage();
  }

  SetupOsThingsAndStuff();

  if (argc-optind != 3) usage();
  loadDictionaries( argv[optind], argv[optind+1] );
  optind += 2;
  load_ciphertext(argv[optind], &len, resume);
  if (len < 3) exit(EXIT_FAILURE);

    assert( hc == 1 );
    if (!resume) {
        if (f == NULL) f = fmin[model];
        if (t == NULL) t = tmax[model];
        if (!set_range(&from, &to, f, t, model)) usage();
    }
    else {
      /* only -o option and --optimizer are allowed in addition to -R */
      int maxargs = (outfile == stdout) ? 5 : 7;
      if ( optimizerOptionPresent ) maxargs += 2;
      if (argc != maxargs) usage();
      if (!set_state(&from, &to, &ckey_res, &gkey_res, &sw_mode, &max_pass, &firstpass, &max_score)) {
        fputs("enigma: error: resume file is not in the right format\n", stderr);
        exit(EXIT_FAILURE);
      }
    }

    clen = (len < CT) ? len : CT;

    InstallSighandler();
    SetupRandomGenerator();
    optimizeScore( &from, &to, &ckey_res, &gkey_res, sw_mode, max_pass, firstpass,
                   max_score, resume, outfile, 1, clen );

  if (outfile != stdout)
    fclose(outfile);
  return 0;

}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
