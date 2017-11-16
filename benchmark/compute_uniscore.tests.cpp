#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

extern "C" {
#include "dict.h"
#include "x86/cipherSsse3.h"
#include "x86/cipherAvx2.h"
#include "cipherNoInterleave_ni.h"
#include "cipherSsse3_ni.h"
#include "cipherAvx_ni.h"
#include "cipherAvx2_ni.h"
}

struct compute_uniscore
    : public MessageAndKeyBasedFixture
{
    const int expectedScore = 66593;
protected:
    void LoadDictionary() override {
        load_unidict( "00unigr.AVv1" );
    }
};

BENCHMARK_DEFINE_F( compute_uniscore, simple ) ( benchmark::State& state ){
    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageBasicNoInterleave( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = UniscoreBasicNoInterleave( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_uniscore, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageSsse3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = UniscoreSsse3( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_uniscore, avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageAvx( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = UniscoreAvx( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_uniscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );
    DecodeMessageAvx2( &key, len );
    int score = 0;
    for( auto _ : state ) {
        score = UniscoreAvx2( len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( compute_uniscore, simple );
BENCHMARK_REGISTER_F( compute_uniscore, ssse3 );
BENCHMARK_REGISTER_F( compute_uniscore, avx );
BENCHMARK_REGISTER_F( compute_uniscore, avx2 );
