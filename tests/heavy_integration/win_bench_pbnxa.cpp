#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <gtest/gtest.h>

#include "dicts/dict_builders.hpp"
#include "dicts/string_dict_loader.hpp"

extern "C" {
#include "OS/Os.h"
#include "ciphertext.h"
#include "echar/echar.h"
#include "enigma/test_data.h"
#include "error.h"
#include "hillclimb.h"
#include "input.h"
#include "randomNumbers.h"
#include "scoreBasic.h"
#include "state.h"
#include "stbrett/krah_optimizer.h"
}

static bool load_tridict( const char text[] ){
    enigma::tri_dict_builder storage{};
    enigma::string_dict_loader tri{ "%3s%d", storage, text };
    return tri.load();
}

static bool load_bidict( const char text[] ){
    enigma::bi_dict_builder storage{};
    enigma::string_dict_loader bi{ "%2s%d", storage, text };
    return bi.load();
}

static void load_message( size_t s,  const char text[] ){
    for( size_t i = 0 ; i < s ; ++i ) {
        ciphertext.plain[i] = make_echar_ascii( text[i] );
    }
}


void err_input_fatal( UNUSED int type ) {
    fprintf( stderr, "%d\n", type );
    throw std::exception();
}


void hillclimb_log( const char *s ) {
    fprintf( stderr, "%s\n", s );
}

volatile sig_atomic_t doShutdown;


TEST( win_bench_pbnxa, krah_basic )
{
    // command line is:
    // enigma.exe -M M3 -c -o bench-result.txt -f "B:532:AA:AAA" -t "B:532:AH:ZZZ" 00trigr.cur 00bigr.cur benchmark_cipher

    ASSERT_TRUE( load_tridict( trigraph_cur ) );
    ASSERT_TRUE( load_bidict( bigraph_cur ) );

    load_message( benchmark_cipher_pbnxa_size, benchmark_cipher_pbnxa );

    Key from;
    Key to;
    {
        auto from_txt = "B:532:AA:AAA";
        auto to_txt = "B:532:AH:ZZZ";
        ASSERT_TRUE( set_range( &from, &to, from_txt, to_txt, EnigmaModel_M3 ) );
    }
    int len = benchmark_cipher_pbnxa_size - 1;
    if( len > CT ) len = CT;
    // make the test deterministic
    srand( 42 );

    Key ckey = from;
    Key gkey = from;
    State state = {
        from :  &from,
        to : &to,
        ckey : &ckey, // not a resume
        gkey : &gkey, // not a resume
        sw_mode : SW_ONSTART,
        pass : 0,
        firstpass : true,
        max_score : INT_MAX - 1,
        ciphertext : ciphertext.plain
    };
    ScoreOptimizer scOptimizer = {
        optimize_score : stbrett_optimize_krah,
        prepare_decoder_lookup : enigma_cipher_decoder_lookup.prepare_decoder_lookup_M_H3,
        score_impl : &enigmaScoreBasic,
    };
    HillclimbersKnapsack knapsack = {
        optimizer : &scOptimizer,
        on_new_best : []( const struct Key*, int ) {},
        save_state : []( const State*, bool ) { /*NOP*/ },
        log : []( const char[] ) {},
    };
    hillclimb(
        &state,
        /* max_pass */      1,
        len,
        &knapsack
    );
    EXPECT_EQ( gkey.score, 17930 );

    EXPECT_EQ( gkey.ukwnum.type, UkwType::UkwType_B );
    auto& slot = gkey.slot;
    EXPECT_EQ( slot.l.type, RingType::RingType_5 );
    EXPECT_EQ( slot.m.type, RingType::RingType_3 );
    EXPECT_EQ( slot.r.type, RingType::RingType_2 );

    std::string stecker;
    std::transform(
        std::begin( gkey.sf.map ),
        std::begin( gkey.sf.map ) + gkey.count,
        std::back_inserter( stecker ),
        []( const echar& e ){ return echar_to_ALPHA( e ); }
    );
    EXPECT_EQ( stecker, "AIBECJDRFYGOHZMUNQPVST" );

    char ring[]{
        echar_delta_to_ALPHA( gkey.ring.l ),
        echar_delta_to_ALPHA( gkey.ring.m ),
        echar_delta_to_ALPHA( gkey.ring.r ), '\0' };
    EXPECT_STREQ( ring, "AAC" );
    char mesg[]{
        echar_delta_to_ALPHA( gkey.mesg.l ),
        echar_delta_to_ALPHA( gkey.mesg.m ),
        echar_delta_to_ALPHA( gkey.mesg.r ), '\0' };
    EXPECT_STREQ( mesg, "HVS" );
}
