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

struct biscore
    : public MessageAndKeyBasedFixture
{
    const int expectedScore = 7913;
protected:
    void LoadDictionary() override {
        if( !enigma::strload_bidict( bigraph_cur ) ) {
            throw enigma::strload_error( "failed to load bigraph_cur" );
        }
    }
};

BENCHMARK_DEFINE_F( biscore, simple )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSimple.biscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( biscore, basic_no_interleave )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreOptNoInterleave.biscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( biscore, basic )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreBasic.biscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( biscore, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSsse3.biscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( biscore, avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx.biscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( biscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx2.biscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( biscore, simple );
BENCHMARK_REGISTER_F( biscore, basic_no_interleave );
BENCHMARK_REGISTER_F( biscore, basic );
BENCHMARK_REGISTER_F( biscore, ssse3 );
BENCHMARK_REGISTER_F( biscore, avx );
BENCHMARK_REGISTER_F( biscore, avx2 );
