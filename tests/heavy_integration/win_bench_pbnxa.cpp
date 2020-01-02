#include <cstdio>
#include <exception>
#include <gtest/gtest.h>
#include <tuple>

#include "Hillclimb_PBNXA.hpp"

extern "C" {
#include "error.h"
#include "scoreBasic.h"
}


void err_input_fatal( UNUSED int type ) {
    fprintf( stderr, "%d\n", type );
    throw std::exception();
}


volatile sig_atomic_t doShutdown;


TEST_P( Hillclimb_PBNXA, KrahOptimizesAsOriginal ) {
    RunHillclimb();
    RunAssertions();
}


auto basic = std::make_tuple( enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3,
                              &enigmaScoreBasic );


INSTANTIATE_TEST_CASE_P( WinBench, Hillclimb_PBNXA,
                         ::testing::Values( basic ) );
