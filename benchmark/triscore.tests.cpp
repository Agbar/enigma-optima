#include <benchmark/benchmark.h>

extern "C" {
#include "charmap.h"
#include "ciphertext.h"
#include "dict.h"
#include "input.h"
#include "scoreBasic.h"
#include "stecker.h"
}

static void triscore_basic( benchmark::State& state ) {
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


    RingsState slot{ /*.g:*/ 0, /*.l:*/ 5, /*.m:*/3, /*.r:*/ 2 };
    RingsState ring{ /*.g:*/ 0, /*.l:*/ 0, /*.m:*/0, /*.r:*/ 0 }; // AAA
    RingsState mesg{ /*.g:*/ 0, /*.l:*/ 0, /*.m:*/'V' - 'A', /*.r:*/ 0 }; // AVA
    Key key {
        // .stbrett:
        { },
        // .slot:
        slot,
        // .ring:
        ring,
        // .mesg:
        mesg,
        // .ukwnum:
        'B' - 'A', // B
        // .model:
        EnigmaModel_M3,
        // sf:
        { },
        // count:
        0,
        // score:
        0 };
    Fill0To25( key.stbrett.letters );
    Fill0To25( key.sf );
    char stecker[] = "AXBZCNDPEOJMLSQV";
    set_stecker( &key, stecker );
    get_stecker( &key );

    scoreLength_t len = sizeof ct - 1;

    enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3( &key, len );

    int score = 0;
    while( state.KeepRunning() ) {
        score = enigmaScoreBasic.triscore( &key, len );
    }

    if( score != 46438 ) {
        state.SkipWithError( "Wrong score!" );
    }

    state.SetBytesProcessed( state.iterations() * sizeof ct );
}

BENCHMARK( triscore_basic );
