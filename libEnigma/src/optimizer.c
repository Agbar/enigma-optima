#include <stdlib.h>
#include <string.h>

#include "optimizer.h"
#include "dict.h"
#include "hillclimb.h"
#include "hillclimb2.h"
#include "stbrett/optimizer.h"
#include "stbrett/krah_optimizer.h"
#include "stbrett/ve3nea_optimizer.h"

typedef void OptimizerFun( const struct Key *from
                  , const struct Key *to
                  , const struct Key *ckey_res
                  , const struct Key *gkey_res
                  , int sw_mode
                  , int max_pass
                  , int firstpass
                  , int max_score
                  , int resume
                  , FILE *outfile
                  , int act_on_sig
                  , int len
                  , stbrett_optimize_f* optimizer );

static OptimizerFun* selectedOptimizer = hillclimb;
static stbrett_optimize_f* stbrettOptimzier = stbrett_optimize_krah;

bool selectOptimizer( const char* const name ) {

    const size_t maxCnt = 10;
    if ( strncmp( name, "AV", maxCnt ) == 0) {

        selectedOptimizer = hillclimb2;
        stbrettOptimzier = stbrett_optimize_ve3nea;
        return true;
    }
    if ( strncmp( name, "Krah", maxCnt ) == 0 ) {
        selectedOptimizer = hillclimb;
        stbrettOptimzier = stbrett_optimize_krah;
        return true;
    }
    return false;
}

void loadDictionaries( const char * const firstDict, const char * const secondDict ) {
    if ( selectedOptimizer == NULL ) {
        exit( 1 );
    }
    if ( selectedOptimizer == hillclimb ) {
        load_tridict( firstDict );
        load_bidict( secondDict );
        return;
    }
    if ( selectedOptimizer == hillclimb2 ) {
        load_tridict( firstDict );
        load_unidict( secondDict );
        return;
    }
    exit( 1 );
}

void optimizeScore( const struct Key *from
                  , const struct Key *to
                  , const struct Key *ckey_res
                  , const struct Key *gkey_res
                  , int sw_mode
                  , int max_pass
                  , int firstpass
                  , int max_score
                  , int resume
                  , FILE *outfile
                  , int act_on_sig
                  , int len ) {

    if( selectedOptimizer == NULL ) {
        exit( 1 );
    }
    selectedOptimizer( from, to, ckey_res, gkey_res, sw_mode,
                       max_pass, firstpass, max_score,
                       resume, outfile, act_on_sig, len,
                       stbrettOptimzier );
}
