#pragma once

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

using TriscoreFun = int (*) ( const union ScoringDecodedMessage* msg, scoreLength_t len );


class TriscoreTestSuite
    : public ::testing::TestWithParam<TriscoreFun> {
public:
    int ComputeTriscore( scoreLength_t len ){
        return scoreFun( &msg, len );
    }

protected:
    void SetUp() override {
        reset_tridict( tridict );
        scoreFun = GetParam();
    }
    ScoringDecodedMessage& Message(){
        return msg;
    }
private:
    TriscoreFun scoreFun {};
    ScoringDecodedMessage msg {};
};


class LengthTriscoreTestSuite
    : public ::testing::TestWithParam<std::tuple<TriscoreFun, scoreLength_t>> {
public:
    int ComputeTriscore() {
        return scoreFun( &msg, len );
    }
protected:
    void SetUp() override {
        reset_tridict( tridict );
        std::tie( scoreFun, len ) = GetParam();
    }
    scoreLength_t MsgLength() const {
        return len;
    }
private:
    TriscoreFun scoreFun {};
    scoreLength_t len {};
    ScoringDecodedMessage msg {};
};
