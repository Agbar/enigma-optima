#include <benchmark/benchmark.h>
#include "MessageAndKeyBasedFixture.h"

#include "load/str_dict.hpp"
#include "load/strload_error.hpp"

extern "C" {
#include "enigma/test_data.h"
#include "x86/cipherSsse3.h"
#include "x86/cipherAvx2.h"
#include "cipherSimple_ni.h"
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
        if( !enigma::strload_unidict( unigraph_AVv1 ) ) {
            throw enigma::strload_error( "failed to load unigraph_AVv1" );
        }
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

BENCHMARK_REGISTER_F( compute_uniscore, simple );
