#include <gtest/gtest.h>


extern "C" {
#include "hillclimb.h"
#include "input.h"
#include "key.h"
}


static uint64_t cnt = 0;


TEST( hillclimb, m3_keys_iteration ) {
    cnt = 0;
    Key from;
    Key to;
    ASSERT_TRUE( set_range( &from,
                            &to,
                            "B:875:MM:YZI",
                            "C:123:AB:JSN",
                            EnigmaModel_M3 ) );
    Key gkey = from;
    Key ckey = from;
    ScoreOptimizer optimizer{
        optimize_score : []( const echar[ 26 ], Key*, int, const enigma_score_function_t* ) { return 1u; },
        prepare_decoder_lookup : []( const Key*, int ) {},
        score_impl : nullptr
    };
    State state{
        from : &from,
        to : &to,
        ckey : &ckey,
        gkey : &gkey,
        sw_mode : {SwMode::SW_ALL},
        pass : 1,
        firstpass : true,
        ciphertext : ciphertext.plain,
    };
    HillclimbersKnapsack knapsack{
        optimizer : &optimizer,
        on_new_best : []( const Key*, int ) {},
        save_state : []( const State*, bool ) {},
        check_shutdown : []( const State* ) {},
        log : []( const char[] ) {},
        scrambler_state_is_endloop : []( const struct Key*, int ) { cnt++; return true; },
    };
    constexpr int max_pass = 2;
    constexpr int len = 100;

    hillclimb( &state, max_pass, len, &knapsack );
    ASSERT_EQ( cnt, max_pass * 3223668 );
}


TEST( hillclimb, m4_keys_iteration ) {
    cnt = 0;
    Key from;
    Key to;
    ASSERT_TRUE( set_range( &from,
                            &to,
                            "B:G876:MC:ZABA",
                            "C:B123:AB:BCDE",
                            EnigmaModel_M4 ) );
    Key gkey = from;
    Key ckey = from;
    ScoreOptimizer optimizer{
        optimize_score : []( const echar[ 26 ], Key*, int, const enigma_score_function_t* ) { return 1u; },
        prepare_decoder_lookup : []( const Key*, int ) {},
        score_impl : nullptr
    };
    State state{
        from : &from,
        to : &to,
        ckey : &ckey,
        gkey : &gkey,
        sw_mode : {SwMode::SW_ALL},
        pass : 1,
        firstpass : true,
        ciphertext : ciphertext.plain,
    };
    HillclimbersKnapsack knapsack{
        optimizer : &optimizer,
        on_new_best : []( const Key*, int ) {},
        save_state : []( const State*, bool ) {},
        check_shutdown : []( const State* ) {},
        log : []( const char[] ) {},
        scrambler_state_is_endloop : []( const struct Key*, int ) { cnt++; return true; },
    };
    constexpr int max_pass = 2;
    constexpr int len = 100;

    hillclimb( &state, max_pass, len, &knapsack );
    ASSERT_EQ( cnt, max_pass * 5063297 );
}
