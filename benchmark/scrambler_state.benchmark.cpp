#include <benchmark/benchmark.h>

#include <emmintrin.h>


extern "C" {
#include "cipher.h"
#include "input.h"
}


static void scrambler_state_ONSTART( benchmark::State& state ) {
    _mm_clflush( (void*)&scrambler_state );
    Key k = {};
    set_key( &k, "B:537:AA:AVA", EnigmaModel_M3, false );

    SwMode mode;
    for( auto _ : state ) {
        benchmark::DoNotOptimize( mode = scrambler_state( &k, state.range( 0 ) ) );
    }
    if( mode.mode != SwMode::SW_ONSTART ) state.SkipWithError( std::to_string( mode.mode ).c_str() );
    state.SetComplexityN( state.range( 0 ) );
}


static void scrambler_state_NONE( benchmark::State& state ) {
    _mm_clflush( (void*)&scrambler_state );
    Key k = {};
    set_key( &k, "B:532:AA:AWF", EnigmaModel_M3, false );

    SwMode mode;
    for( auto _ : state ) {
        benchmark::DoNotOptimize( mode = scrambler_state( &k, state.range( 0 ) ) );
    }
    if( mode.mode != SwMode::SW_NONE ) state.SkipWithError( std::to_string( mode.mode ).c_str() );
    state.SetComplexityN( state.range( 0 ) );
}


static void scrambler_state_NONE_double_notch( benchmark::State& state ) {
    _mm_clflush( (void*)&scrambler_state );
    Key k = {};
    set_key( &k, "B:536:AA:AWA", EnigmaModel_M3, false );

    SwMode mode;
    for( auto _ : state ) {
        benchmark::DoNotOptimize( mode = scrambler_state( &k, state.range( 0 ) ) );
    }
    if( mode.mode != SwMode::SW_NONE ) state.SkipWithError( std::to_string( mode.mode ).c_str() );
    state.SetComplexityN( state.range( 0 ) );
}


BENCHMARK( scrambler_state_ONSTART )->RangeMultiplier( 2 )->Range( 1, 250 )->Complexity();
BENCHMARK( scrambler_state_NONE )->RangeMultiplier( 2 )->Range( 1, 650 )->Complexity();
BENCHMARK( scrambler_state_NONE_double_notch )->RangeMultiplier( 2 )->Range( 1, 325 )->Complexity();
