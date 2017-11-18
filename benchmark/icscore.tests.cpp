#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

extern "C" {
#include "scoreBasic.h"
#include "scoreNoInterleave.h"
#include "scoreSimple.h"
#include "x86/cipherSsse3.h"
#include "x86/scoreSsse3.h"
#include "x86/cipherAvx2.h"
#include "x86/scoreAvx2.h"
}

struct icscore
    : public MessageAndKeyBasedFixture
{
protected:
    void LoadDictionary() override {}
};

BENCHMARK_DEFINE_F( icscore, simple )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSimple.icscore( &key, len );
    }
    if( score != 1344 ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( icscore, basic_no_interleave )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreOptNoInterleave.icscore( &key, len );
    }
    if( score != 1344 ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( icscore, basic )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreBasic.icscore( &key, len );
    }
    if( score != 1344 ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( icscore, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreSsse3.icscore( &key, len );
    }
    if( score != 1344 ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( icscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = enigmaScoreAvx2.icscore( &key, len );
    }
    if( score != 1344 ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( icscore, simple );
BENCHMARK_REGISTER_F( icscore, basic_no_interleave );
BENCHMARK_REGISTER_F( icscore, basic );
BENCHMARK_REGISTER_F( icscore, ssse3 );
BENCHMARK_REGISTER_F( icscore, avx2 );
