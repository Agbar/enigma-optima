#include <gtest/gtest.h>

#include "computeScore.tests.hpp"
#include "../cpuFeatures.hpp"

extern "C"{
    #include "x86/computeScoreSse2.h"
    #include "x86/computeScoreSse2Vex.h"
}

using ::testing::Combine;
using ::testing::Range;
using ::testing::Values;

static auto triscoreFun    = Values( &ComputeTriscoreFromDecodedMsgSse2 );
static auto triscoreFunVex = Values( &ComputeTriscoreFromDecodedMsgSse2Vex );


INSTANTIATE_TEST_CASE_P(
    TriscoreSse2,
    TriscoreTestSuite,
    Combine(
        Values( &isSse2Supported ),
        triscoreFun )
);

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2,
    LengthTriscoreTestSuite,
    Combine(
        Values( &isSse2Supported ),
        triscoreFun,
        Range<scoreLength_t>( 3, CT ) )
);

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2Vex,
    TriscoreTestSuite,
    Combine(
        Values( &isSse2VexSupported ),
        triscoreFunVex )
);

INSTANTIATE_TEST_CASE_P(
    TriscoreSse2Vex,
    LengthTriscoreTestSuite,
    Combine(
        Values( &isSse2VexSupported ),
        triscoreFunVex,
        Range<scoreLength_t>( 3, CT ) )
);