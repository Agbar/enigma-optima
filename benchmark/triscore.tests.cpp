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

struct triscore
    : public MessageAndKeyBasedFixture
{
    const int expectedScore = 46438;
protected:
    void LoadDictionary() override {
        if( !enigma::strload_tridict( trigraph_AVv1 ) ) {
            throw enigma::strload_error( "failed to load trigraph_AVv1" );
        }
    }
};

BENCHMARK_DEFINE_F( triscore, simple )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSimple.triscore( &key, len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( triscore, basic_no_interleave )( benchmark::State& state ) {
    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreOptNoInterleave.triscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( triscore, basic )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreBasic.triscore( &key, len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( triscore, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSsse3.triscore( &key, len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( triscore, avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx.triscore( &key, len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( triscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx2.triscore( &key, len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( triscore, simple );
BENCHMARK_REGISTER_F( triscore, basic_no_interleave );
BENCHMARK_REGISTER_F( triscore, basic );
BENCHMARK_REGISTER_F( triscore, ssse3 );
BENCHMARK_REGISTER_F( triscore, avx );
BENCHMARK_REGISTER_F( triscore, avx2 );
