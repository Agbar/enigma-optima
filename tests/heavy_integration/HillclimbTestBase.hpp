#pragma once

#include <gtest/gtest.h>

extern "C"{
#include "cipher.h"
#include "hillclimb.h"
}


using IsSupportedFun = bool ( * )();
using ScoringImplParams = std::tuple< enigma_cipher_function_t*,
                                      enigma_score_function_t*,
                                      IsSupportedFun >;


class HillclimbTestBase
    : public ::testing::TestWithParam< ScoringImplParams > {

    ScoreOptimizer scOptimizer = {};
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

protected:
    void SetUp() final;
    virtual const stbrett_optimize_f& GetStbrettOptimizer() = 0;
    static void LoadMessage( int& length );
    static void SetKeyRange( Key& from, Key& to );
    static void ClearMessage();

public:
    bool IsSupported() const noexcept {
        return isSupportedFun();
    }
    void RunHillclimb();
    void RunAssertions();
};
