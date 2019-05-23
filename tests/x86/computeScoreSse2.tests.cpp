#include <gtest/gtest.h>

#include "computeScore.tests.hpp"

extern "C"{
    #include "x86/computeScoreSse2.h"
}

static auto triscoreFun =::testing::Values( &ComputeTriscoreFromDecodedMsgSse2 );

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2,
    TriscoreTestSuite,
    triscoreFun );

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2,
    LengthTriscoreTestSuite,
    ::testing::Combine(
        triscoreFun,
        ::testing::Range<scoreLength_t>( 3, CT ) )
);