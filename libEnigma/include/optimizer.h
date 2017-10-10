#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "key.h"

bool selectOptimizer( const char* const name );

void optimizeScore( const Key *from
                  , const Key *to
                  , const Key *ckey_res
                  , const Key *gkey_res
                  , int sw_mode
                  , int max_pass
                  , int firstpass
                  , int max_score
                  , int resume
                  , FILE *outfile
                  , int act_on_sig
                  , int len );
