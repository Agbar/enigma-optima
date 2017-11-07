#include <benchmark/benchmark.h>

extern "C" {
#include "charmap.h"
#include "ciphertext.h"
#include "dict.h"
#include "input.h"
#include "scoreBasic.h"
#include "scoreNoInterleave.h"
#include "scoreSimple.h"
#include "stecker.h"
#include "x86/cipherSsse3.h"
#include "x86/scoreSsse3.h"
#include "x86/cipherAvx2.h"
#include "x86/scoreAvx2.h"
}

struct icscore
    : public benchmark::Fixture {
    size_t len = 0;
    Key key {};

    void SetUp( benchmark::State& st ) override {
        init_charmap();
        // Score: 46438
        // UKW: B       // ukwnum
        // W/0: 532     // slot
        // Stecker: AXBZCNDPEOJMLSQV
        // Rings: AAA
        // Message key: AVA

        const unsigned char ct[] = "pbnxasmdaxnooyhrczgvvzcbigibgwhm"
                                   "xkrrvqcfjczptunswaddstigqqcsagpk"
                                   "rxxlomgfxaphhmrfsdkytmypmvrohasq"
                                   "yrwfwvavgccudbibxxdyzsacjsyotmwu"
                                   "cnwomhhjpywdcclupgswclmbczssyxpg"
                                   "mgmqxaufulnozeqenheizzaklc";

        for( size_t i = 0 ; i < sizeof ct; ++i ) {
            ciphertext.plain[i] = code[ct[i]];
        }

        key.slot = { /*.g:*/ 0, /*.l:*/ 5, /*.m:*/3, /*.r:*/ 2 };
        key.ring = { /*.g:*/ 0, /*.l:*/ 0, /*.m:*/0, /*.r:*/ 0 }; // AAA
        key.mesg = { /*.g:*/ 0, /*.l:*/ 0, /*.m:*/'V' - 'A', /*.r:*/ 0 }; // AVA
        key.ukwnum = 'B' - 'A';
        key.model = EnigmaModel_M3;

        Fill0To25( key.stbrett.letters );
        Fill0To25( key.sf );
        char stecker[] = "AXBZCNDPEOJMLSQV";
        set_stecker( &key, stecker );
        get_stecker( &key );

        len = sizeof ct - 1;
    }
};

BENCHMARK_DEFINE_F( icscore, simple )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    while( state.KeepRunning() ) {
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
    while( state.KeepRunning() ) {
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
    while( state.KeepRunning() ) {
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
    while( state.KeepRunning() ) {
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
    while( state.KeepRunning() ) {
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
