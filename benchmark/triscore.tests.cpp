#include <benchmark/benchmark.h>

extern "C" {
#include "charmap.h"
#include "ciphertext.h"
#include "dict.h"
#include "input.h"
#include "scoreBasic.h"
#include "stecker.h"
#include "x86/cipherSsse3.h"
#include "x86/scoreSsse3.h"
}


struct triscore
    : public benchmark::Fixture {
    size_t len = 0;
    Key key {};

    triscore() {
        init_charmap();

        load_tridict( "00trigr.AVv1" );

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

BENCHMARK_DEFINE_F( triscore, basic )( benchmark::State& state ) {

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    while( state.KeepRunning() ) {
        score = enigmaScoreBasic.triscore( &key, len );
    }

    if( score != 46438 ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( triscore, basic );

BENCHMARK_DEFINE_F( triscore, ssse3 ) ( benchmark::State& state ){
    enigma_cipher_decoder_lookup_ssse3.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    while( state.KeepRunning() ) {
        score = enigmaScoreSsse3.triscore( &key, len );
    }

    if( score != 46438 ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * len );
}

BENCHMARK_REGISTER_F( triscore, ssse3 );
