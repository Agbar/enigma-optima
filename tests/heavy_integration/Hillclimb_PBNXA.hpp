#pragma once

#include <gtest/gtest.h>

extern "C"{
#include "cipher.h"
#include "hillclimb.h"
#include "stbrett/krah_optimizer.h"
}

class Hillclimb_PBNXA
    : public ::testing::TestWithParam<
          std::tuple< enigma_cipher_function_t*,
                      enigma_score_function_t* > > {

    ScoreOptimizer scOptimizer = {
        optimize_score : stbrett_optimize_krah,
        prepare_decoder_lookup : nullptr,
        score_impl : nullptr,
    };
    const HillclimbersKnapsack knapsack = {
        optimizer : &scOptimizer,
        on_new_best : []( const struct Key*, int ) {},
        save_state : []( const State*, bool ) { /*NOP*/ },
        log : []( const char[] ) {},
    };
    Key gkey{};

    static Key from;
    static Key to;
    static int len;

public:
    static void SetUpTestCase();

protected:
    void SetUp() final;

public:
    void RunHillclimb();
    void RunAssertions();
};
