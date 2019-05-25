#pragma once

#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>

extern "C"{
    #include "dict.h"
    #include "score.h"
}

template< size_t last_dim_size, size_t dim_size >
void reset_tridict( dict_t (&td)[last_dim_size][dim_size][dim_size] ){
    std::memset( &td, 0, sizeof td );
}

using IsSupportedFun = bool (*) ();
using TriscoreFun = uint32_t (*) ( const union ScoringDecodedMessage* msg, scoreLength_t len );


class TriscoreTestSuite
    : public ::testing::TestWithParam<std::tuple<IsSupportedFun, TriscoreFun>> {
public:
    int ComputeTriscore( scoreLength_t len ){
        return scoreFun( &msg, len );
    }

protected:
    void SetUp() override {
        reset_tridict( tridict );
        std::tie( isSupportedFun, scoreFun ) = GetParam();
    }

    ScoringDecodedMessage& Message() noexcept {
        return msg;
    }

    bool IsSupported() const noexcept {
        return isSupportedFun();
    }
private:
    IsSupportedFun isSupportedFun {};
    TriscoreFun scoreFun {};
    ScoringDecodedMessage msg {};
};


class LengthTriscoreTestSuite
    : public ::testing::TestWithParam<std::tuple<IsSupportedFun, TriscoreFun, scoreLength_t>> {
public:
    int ComputeTriscore() {
        return scoreFun( &msg, len );
    }
protected:
    void SetUp() override {
        reset_tridict( tridict );
        std::tie( isSupportedFun, scoreFun, len ) = GetParam();
    }

    scoreLength_t MsgLength() const noexcept {
        return len;
    }

    bool IsSupported() const noexcept {
        return isSupportedFun();
    }
private:
    IsSupportedFun isSupportedFun {};
    TriscoreFun scoreFun {};
    scoreLength_t len {};
    ScoringDecodedMessage msg {};
};
