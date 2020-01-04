#include <cstdio>
#include <exception>
#include <gtest/gtest.h>
#include <tuple>

#include "Hillclimb_PBNXA.hpp"

extern "C" {
#include "error.h"
#include "scoreBasic.h"
#include "scoreNoInterleave.h"
#include "scoreSimple.h"
#include "x86/cipherAvx2.h"
#include "x86/cipherSsse3.h"
#include "x86/scoreAvx.h"
#include "x86/scoreAvx2.h"
#include "x86/scoreSsse3.h"
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


static auto basic = std::make_tuple( &enigma_cipher_decoder_lookup, &enigmaScoreBasic );
static auto simple = std::make_tuple( &enigma_cipher_decoder_lookup, &enigmaScoreSimple );
static auto noInterleave = std::make_tuple ( &enigma_cipher_decoder_lookup, &enigmaScoreOptNoInterleave );
static auto ssse3 = std::make_tuple( &enigma_cipher_decoder_lookup_ssse3, &enigmaScoreSsse3 );
static auto avx = std::make_tuple( &enigma_cipher_decoder_lookup_ssse3, &enigmaScoreAvx );
static auto avx2 = std::make_tuple( &enigma_cipher_DecoderLookupAvx2, &enigmaScoreAvx2 );


namespace testing {

template<>
::std::string PrintToString< ScoringImplParams >( const ScoringImplParams& value ) {
    enigma_score_function_t* score_fun;
    std::tie( std::ignore, score_fun ) = value;
    if( value == basic ) return "basic";
    if( value == simple ) return "simple";
    if( value == noInterleave ) return "basic_no_interleave";
    if( value == ssse3 ) return "ssse3";
    if( value == avx ) return "avx";
    if( value == avx2 ) return "avx2";
    throw std::invalid_argument( "value not recognized as enigma_score_function_t" );
}

}


INSTANTIATE_TEST_CASE_P( WinBench, Hillclimb_PBNXA,
                         ::testing::Values( basic, simple, noInterleave, ssse3, avx, avx2 ),
                         ::testing::PrintToStringParamName() );
