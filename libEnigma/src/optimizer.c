#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "error.h"
#include "hillclimb.h"
#include "optimizer.h"
#include "state.h"
#include "stbrett/krah_optimizer.h"
#include "stbrett/optimizer.h"
#include "stbrett/ve3nea_optimizer.h"

static stbrett_optimize_f* stbrettOptimzier = stbrett_optimize_krah;


static void nop_log( UNUSED const char msg[] ) {
    // NOP
}


bool selectOptimizer( const char* const name ) {

    const size_t maxCnt = 10;
    if ( strncmp( name, "AV", maxCnt ) == 0) {
        stbrettOptimzier = stbrett_optimize_ve3nea;
        return true;
    }
    if ( strncmp( name, "Krah", maxCnt ) == 0 ) {
        stbrettOptimzier = stbrett_optimize_krah;
        return true;
    }
    return false;
}

void loadDictionaries( const char * const firstDict, const char * const secondDict ) {
    if ( stbrettOptimzier == NULL ) {
        exit( 1 );
    }
    if ( stbrettOptimzier == stbrett_optimize_krah ) {
        load_tridict( firstDict );
        load_bidict( secondDict );
        return;
    }
    if ( stbrettOptimzier == stbrett_optimize_ve3nea ) {
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
                  , int len ) {

    if( stbrettOptimzier == NULL ) {
        exit( 1 );
    }

    struct Key ckey = resume ? *ckey_res : *from;
    struct Key gkey = resume ? *gkey_res : *from;

    struct State state = {
        .from = from,
        .to = to,
        .ckey = &ckey,
        .gkey = &gkey,
        .sw_mode = sw_mode,
        .firstpass = firstpass,
        .max_score = max_score,
        .ciphertext = ciphertext.plain
    };

    struct ScoreOptimizer optimizer = {
        .optimize_score = stbrettOptimzier,
    };
    enigma_score_init( enigma_cpu_flags, optimizer.score_impl );
    enigma_cipher_init( enigma_cpu_flags, from->model,
                        &optimizer.prepare_decoder_lookup );

    struct HillclimbersKnapsack knapsack = {
        .optimizer = &optimizer,
        .save_state = save_state,
        .log = resume ? hillclimb_log : nop_log,
    };
    hillclimb( &state,
               max_pass,
               outfile, len,
               &knapsack );
}
