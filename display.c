#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "config/releaseVersion.h"

void usage(void)
{
  fprintf(stderr, "enigma: usage: enigma \
[ -hvR ] \
[ --optimizer algorithm ]\
[ -o output-file ] \
trigram-dictionary 2nd-dictionary ciphertext \
\n\nUse enigma -h for detailed help\n\n");

  exit(EXIT_FAILURE);
}

void help(void)
{
  fprintf(stderr, "\nOptions:\n\n\
-o      Output file. Defaults to stdout.\n\n\
        arguments <trigram-dict>, <2nd-dict> and <ciphertext>.\n\n\
-R      Standalone option:  Resume a hill climb, reading the previous state from\n\
        00hc.resume.\n\n\
--optimizer <algorithm> Selects optimization algorithm. Available options are:\n\
        * AV   - algorithm by Alex VE3NEA used in Enigma AV\n\
        * Krah - (default) original algorithm used in Enigma 0.76\n\
        This option can be used together with -R.\n\n\
-h      Display this help screen.\n\n\
-v      Display version information.\n" );

  exit(EXIT_SUCCESS);
}


void version(void)
{
  fprintf(stdout, "enigma-optima version %s\n\
Copyright (C) 2005 Stefan Krah <stefan@bytereef.org>\n\
          (C) 2016 Agbar <https://github.com/Agbar>\n\n\
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
To report bugs or offer suggestions, please use GitHub.\n\
    https://github.com/Agbar/enigma-optima/issues\n\n"
    , releaseVersion );

  exit(EXIT_SUCCESS);
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
