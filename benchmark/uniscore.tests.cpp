#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

#include "load/str_dict.hpp"
#include "load/strload_error.hpp"

extern "C" {
#include "enigma/test_data.h"
#include "scoreBasic.h"
#include "scoreNoInterleave.h"
#include "scoreSimple.h"
#include "x86/cipherSsse3.h"
#include "x86/scoreSsse3.h"
#include "x86/scoreAvx.h"
#include "x86/cipherAvx2.h"
#include "x86/scoreAvx2.h"
}

struct uniscore
    : public MessageAndKeyBasedFixture
{
    const int expectedScore = 66593;
protected:
    void LoadDictionary() override {
        if( !enigma::strload_unidict( unigraph_AVv1 ) ) {
            throw enigma::strload_error( "failed to load unigraph_AVv1" );
        }
    }
};

BENCHMARK_DEFINE_F( uniscore, simple )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSimple.uniscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( uniscore, basic_no_interleave )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreOptNoInterleave.uniscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( uniscore, basic )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreBasic.uniscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( uniscore, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSsse3.uniscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( uniscore, avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx.uniscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( uniscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx2.uniscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( uniscore, simple );
BENCHMARK_REGISTER_F( uniscore, basic_no_interleave );
BENCHMARK_REGISTER_F( uniscore, basic );
BENCHMARK_REGISTER_F( uniscore, ssse3 );
BENCHMARK_REGISTER_F( uniscore, avx );
BENCHMARK_REGISTER_F( uniscore, avx2 );
