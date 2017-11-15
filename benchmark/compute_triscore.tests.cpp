#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

extern "C" {
#include "dict.h"
#include "x86/cipherSsse3.h"
#include "x86/cipherAvx2.h"
#include "cipherAvx_ni.h"
#include "cipherAvx2_ni.h"
#include "cipherSsse3_ni.h"
}

struct compute_triscore
    : public MessageAndKeyBasedFixture
{
protected:
    void LoadDictionary() override {
        load_tridict( "00trigr.AVv1" );
    }
};

BENCHMARK_DEFINE_F( compute_triscore, ssse3_sse2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageSsse3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = TriscoreSse3( len );
    }

    if( score != 46438 ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_triscore, avx_sse2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageAvx( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = TriscoreAvx( len );
    }

    if( score != 46438 ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_triscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );
    DecodeMessageAvx2( &key, len );
    int score = 0;
    for( auto _ : state ) {
        score = TriscoreAvx2( len );
    }
    if( score != 46438 ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( compute_triscore, ssse3_sse2 );
BENCHMARK_REGISTER_F( compute_triscore, avx_sse2 );
BENCHMARK_REGISTER_F( compute_triscore, avx2 );

