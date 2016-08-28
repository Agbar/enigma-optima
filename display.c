#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void usage(void)
{
  fprintf(stderr, "enigma: usage: enigma \
[ -hvicxaR ] \
[ -M model ] \
[ -u umkehrwalze ] \
[ -w wheel-order ] \
[ -r ring-settings ] \
[ -s stecker-pairs ] \
[ -m message-key ] \
[ -o output-file ] \
[ -n passes ] \
[ -z maximum-score ] \
[ -k single-key ] \
[ -f lower-bound ] \
[ -t upper-bound ] \
[ trigram-dictionary bigram-dictionary ciphertext ] \
\n\nUse enigma -h for detailed help\n\n");

  exit(EXIT_FAILURE);
}


void help(void)
{
  fprintf(stderr, "\nOptions:\n\n\
-o      Output file. Defaults to stdout.\n\n\
        arguments <trigram-dict>, <bigram-dict> and <ciphertext>.\n\n\
-R      Standalone option:  Resume a hill climb, reading the previous state from\n\
        00hc.resume.\n\n\
-h      Display this help screen.\n\n\
-v      Display version information.\n" );

  exit(EXIT_SUCCESS);
}


void version(void)
{
  fprintf(stdout, "enigma-suite version 0.76\n\
Copyright (C) 2005 Stefan Krah <stefan@bytereef.org>\n\n\
    This program is free software; you can redistribute it and/or modify it\n\
    under the terms of version 2 (only) of the GNU General Public License as\n\
    published by the Free Software Foundation.\n\n\
    This program is distributed in the hope that it will be useful,\n\
    but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
    GNU General Public License for more details.\n\n\
    You should have received a copy of the GNU General Public License\n\
    along with this program; if not, write to the Free Software\n\
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.\n\n\
To report bugs or offer suggestions, please mail to <enigma-suite@bytereef.org>.\n\n");

  exit(EXIT_SUCCESS);
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
