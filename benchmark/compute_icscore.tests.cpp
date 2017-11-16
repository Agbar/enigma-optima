#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

extern "C" {
#include "x86/cipherSsse3.h"
#include "x86/cipherAvx2.h"
#include "cipherNoInterleave_ni.h"
#include "cipherSsse3_ni.h"
#include "cipherAvx_ni.h"
#include "cipherAvx2_ni.h"
}

struct compute_icscore
    : public MessageAndKeyBasedFixture
{
    const int expectedScore = 1344;
protected:
    void LoadDictionary() override {}
};

BENCHMARK_DEFINE_F( compute_icscore, simple ) ( benchmark::State& state ){
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );
    DecodeMessageBasicNoInterleave( &key, len );
    int score = 0;
    for( auto _ : state ) {
        score = IcscoreSimple( len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_icscore, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageSsse3( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = IcscoreSsse3( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_icscore, avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    DecodeMessageAvx( &key, len );

    int score = 0;
    for( auto _ : state ) {
        score = IcscoreAvx( len );
    }

    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( compute_icscore, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );
    DecodeMessageAvx2( &key, len );
    int score = 0;
    for( auto _ : state ) {
        score = IcscoreAvx2( len );
    }
    if( score != expectedScore ) {
        state.SkipWithError( "Wrong score!" );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( compute_icscore, simple );
BENCHMARK_REGISTER_F( compute_icscore, ssse3 );
BENCHMARK_REGISTER_F( compute_icscore, avx );
BENCHMARK_REGISTER_F( compute_icscore, avx2 );

