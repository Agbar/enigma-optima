#pragma once
/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "key.h"
#include "stbrett/optimizer.h"

void hillclimb( const struct Key *from, const struct Key *to, const struct Key *ckey_res, const struct Key *gkey_res,
                int sw_mode, int max_pass, int firstpass, int max_score, int resume,
                FILE *outfile, int act_on_sig, int len,
                stbrett_optimize_f* optimizer );
