#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

#include "load/str_dict.hpp"
#include "load/strload_error.hpp"

extern "C" {
#include "enigma/test_data.h"
#include "x86/cipherSsse3.h"
#include "x86/cipherAvx2.h"
#include "cipherSimple_ni.h"
#include "cipherSse2_ni.h"
#include "cipherSsse3_ni.h"
#include "cipherAvx_ni.h"
#include "cipherAvx2_ni.h"
}

struct compute_biscore
    : public MessageAndKeyBasedFixture
{
    const int expectedScore = 7913;
protected:
    void LoadDictionary() override {
        if( !enigma::strload_bidict( bigraph_1941 ) ) {
            throw enigma::strload_error( "failed to load bigraph_1941" );
        }
    }
};

BENCHMARK_DEFINE_F( compute_biscore, simple ) ( benchmark::State& state ){
    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageBasicNoInterleave( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = BiscoreSimple( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_biscore, sse2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("sse2") ) {
        state.SkipWithError("SSE2 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageSsse3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = BiscoreSse2( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_biscore, sse2_avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );
    DecodeMessageSsse3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = BiscoreAvx( len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( compute_biscore, simple );
BENCHMARK_REGISTER_F( compute_biscore, sse2 );
BENCHMARK_REGISTER_F( compute_biscore, sse2_avx );

