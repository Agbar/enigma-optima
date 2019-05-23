#include <gtest/gtest.h>

#include "computeScore.tests.hpp"

extern "C"{
    #include "x86/computeScoreSse2.h"
    #include "x86/computeScoreSse2Vex.h"
}

static auto triscoreFun    = ::testing::Values( &ComputeTriscoreFromDecodedMsgSse2 );
static auto triscoreFunVex = ::testing::Values( &ComputeTriscoreFromDecodedMsgSse2Vex );

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

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2Vex,
    TriscoreTestSuite,
    triscoreFunVex );

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2Vex,
    LengthTriscoreTestSuite,
    ::testing::Combine(
        triscoreFunVex,
        ::testing::Range<scoreLength_t>( 3, CT ) )
);