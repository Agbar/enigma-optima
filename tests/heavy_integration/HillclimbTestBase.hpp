#pragma once

#include <gtest/gtest.h>
#include <stdexcept>

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
    HillclimbersKnapsack knapsack = {
        optimizer : &scOptimizer,
        on_new_best : &OnNewBestCallback,
        save_state : []( const State*, bool ) { /*NOP*/ },
        log : []( const char[] ) {},
    };
    Key gkey{};
    IsSupportedFun isSupportedFun {};

    static Key from;
    static Key to;
    static int len;
    static HillclimbTestBase* active;

    static void OnNewBestCallback( const struct Key*, int ) {
        if( !active ) throw std::logic_error( "active test not set." );
        active->AssertOnNewBest();
    }

protected:
    void SetUp() final;
    void TearDown() final;
    virtual const stbrett_optimize_f& GetStbrettOptimizer() = 0;
    static void LoadMessage( int& length );
    static void SetKeyRange( Key& from, Key& to );
    static void ClearMessage();

    virtual void RunFinalAssertions() = 0;
    virtual void AssertOnNewBest() const {}

    // properties:
    const Key& GKey() const { return gkey; }

public:
    bool IsSupported() const noexcept {
        return isSupportedFun();
    }
    void RunHillclimb();
    void RunAssertions();
};
