#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "key.h"

bool selectOptimizer( const char* const name );

void loadDictionaries(  const char * firstDict, const char * secondDic );

void optimizeScore( const struct Key *from
                  , const struct Key *to
                  , const struct Key *ckey_res
                  , const struct Key *gkey_res
                  , int sw_mode
                  , int max_pass
                  , int firstpass
                  , int resume
                  , FILE *outfile
                  , int len );
