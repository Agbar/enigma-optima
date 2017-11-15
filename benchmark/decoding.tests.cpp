#include <benchmark/benchmark.h>

#include "MessageAndKeyBasedFixture.h"

extern "C" {
#include "x86/cipherAvx2.h"
#include "x86/cipherSsse3.h"
#include "cipherAvx_ni.h"
#include "cipherAvx2_ni.h"
#include "cipherSsse3_ni.h"
#include "scoreNoInterleave.h"
#include "scoreNoInterleave_inlines.h"
}

struct decoding
    : public MessageAndKeyBasedFixture
{
protected:
    void LoadDictionary() override {}
};

BENCHMARK_DEFINE_F( decoding, simple )( benchmark::State& state ) {
    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    while( state.KeepRunning() ) {
        DecodeScoredMessagePartNoInterleaveSimple( &key, len, &decodedMsgPartNoInterleave );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( decoding, basic_no_interleave )( benchmark::State& state ) {
    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    while( state.KeepRunning() ) {
        DecodeScoredMessagePartNoInterleave( &key, len, &decodedMsgPartNoInterleave );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( decoding, ssse3 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("ssse3") ) {
        state.SkipWithError("SSSE3 not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    while( state.KeepRunning() ) {
        DecodeMessageSsse3( &key, len );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( decoding, avx ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx") ) {
        state.SkipWithError("AVX not supported");
        return;
    }
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    while( state.KeepRunning() ) {
        DecodeMessageAvx( &key, len );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_DEFINE_F( decoding, avx2 ) ( benchmark::State& state ){
    if( !__builtin_cpu_supports("avx2") ) {
        state.SkipWithError("AVX2 not supported");
        return;
    }
    enigma_cipher_DecoderLookupAvx2.prepare_decoder_lookup_M_H3( &key, len );
    while( state.KeepRunning() ) {
        DecodeMessageAvx2( &key, len );
    }
    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( decoding, simple );
BENCHMARK_REGISTER_F( decoding, basic_no_interleave);
BENCHMARK_REGISTER_F( decoding, ssse3 );
BENCHMARK_REGISTER_F( decoding, avx );
BENCHMARK_REGISTER_F( decoding, avx2 );
