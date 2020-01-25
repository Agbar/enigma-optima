#include <cstdio>
#include <exception>
#include <gtest/gtest.h>
#include <tuple>

#include "DictsPolicy.hpp"
#include "HillclimbAssertions.hpp"
#include "HillclimbTest.hpp"
#include "Krah1941Policies.hpp"
#include "Ve3neaAv1Policies.hpp"

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


using PBNXA_Krah1941 = HillclimbTest< Krah1941DictsPolicy, HillclimbAssertions >;
using PBNXA_Ve3neaAv1 = HillclimbTest< Ve3neaAv1DictsPolicy, HillclimbAssertions >;

// WinBench's command line is:
// enigma.exe -M M3 -c -o bench-result.txt -f "B:532:AA:AAA" -t
// "B:532:AH:ZZZ" 00trigr.cur 00bigr.cur benchmark_cipher


TEST_P( PBNXA_Krah1941, Hillclimb ) {
    if( !IsSupported() ) return;
    RunHillclimb();
    RunAssertions();
}


TEST_P( PBNXA_Ve3neaAv1, Hillclimb ) {
    if( !IsSupported() ) return;
    RunHillclimb();
    RunAssertions();
}


static ScoringImplParams basic =
    std::make_tuple( &enigma_cipher_decoder_lookup,
                     &enigmaScoreBasic,
                     []() { return true; } );
static ScoringImplParams simple =
    std::make_tuple( &enigma_cipher_decoder_lookup,
                     &enigmaScoreSimple,
                     []() { return true; } );
static ScoringImplParams noInterleave =
    std::make_tuple( &enigma_cipher_decoder_lookup,
                     &enigmaScoreOptNoInterleave,
                     []() { return true; } );
static ScoringImplParams ssse3 =
    std::make_tuple( &enigma_cipher_decoder_lookup_ssse3,
                     &enigmaScoreSsse3,
                     []() -> bool { return __builtin_cpu_supports( "ssse3" ); } );
static ScoringImplParams avx =
    std::make_tuple( &enigma_cipher_decoder_lookup_ssse3,
                     &enigmaScoreAvx,
                     []() -> bool { return __builtin_cpu_supports( "avx" ); } );
static ScoringImplParams avx2 =
    std::make_tuple( &enigma_cipher_DecoderLookupAvx2,
                     &enigmaScoreAvx2,
                     []() -> bool { return __builtin_cpu_supports( "avx2" ); } );


namespace testing {

template<>
::std::string PrintToString< ScoringImplParams >( const ScoringImplParams& value ) {
    enigma_score_function_t* score_fun;
    std::tie( std::ignore, score_fun, std::ignore ) = value;
    if( value == basic ) return "basic";
    if( value == simple ) return "simple";
    if( value == noInterleave ) return "basic_no_interleave";
    if( value == ssse3 ) return "ssse3";
    if( value == avx ) return "avx";
    if( value == avx2 ) return "avx2";
    throw std::invalid_argument( "value not recognized as enigma_score_function_t" );
}

} // namespace testing


INSTANTIATE_TEST_CASE_P( WinBench, PBNXA_Krah1941,
                         ::testing::Values( basic, simple, noInterleave, ssse3, avx, avx2 ),
                         ::testing::PrintToStringParamName() );


INSTANTIATE_TEST_CASE_P( WinBench, PBNXA_Ve3neaAv1,
                         ::testing::Values( basic, simple, noInterleave, ssse3, avx, avx2 ),
                         ::testing::PrintToStringParamName() );
