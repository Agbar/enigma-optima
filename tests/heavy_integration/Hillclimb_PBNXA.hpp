#pragma once

#include <gtest/gtest.h>

extern "C"{
#include "cipher.h"
#include "hillclimb.h"
#include "stbrett/krah_optimizer.h"
}


using IsSupportedFun = bool ( * )();
using ScoringImplParams = std::tuple< enigma_cipher_function_t*,
                                      enigma_score_function_t*,
                                      IsSupportedFun >;


class Hillclimb_PBNXA
    : public ::testing::TestWithParam< ScoringImplParams > {

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
    IsSupportedFun isSupportedFun {};

    static Key from;
    static Key to;
    static int len;

public:
    static void SetUpTestCase(){
        LoadDicts();
        LoadMessage( len );
        SetKeyRange( from, to );
    }

protected:
    void SetUp() final;
    static void LoadDicts();
    static void LoadMessage( int& length );
    static void SetKeyRange( Key& from, Key& to );

public:
    bool IsSupported() const noexcept {
        return isSupportedFun();
    }
    void RunHillclimb();
    void RunAssertions();
};
