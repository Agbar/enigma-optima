#include <gtest/gtest.h>

#include "computeScore.tests.hpp"

extern "C"{
    #include "x86/computeScoreAvx2.h"
}

static auto triscoreFun =::testing::Values( &ComputeTriscoreFromDecodedMsgAvx2 );

INSTANTIATE_TEST_CASE_P(
    TriscoreAvx2,
    TriscoreTestSuite,
    triscoreFun );

INSTANTIATE_TEST_CASE_P(
    TriscoreAvx2,
    LengthTriscoreTestSuite,
    ::testing::Combine(
        triscoreFun,
        ::testing::Range<scoreLength_t>( 3, CT ) )
);
