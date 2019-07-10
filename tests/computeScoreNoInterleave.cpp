#include <gtest/gtest.h>

#include "x86/computeScore.tests.hpp"

extern "C" {
#include "computeScoreNoInterleave.h"
}

using ::testing::Combine;
using ::testing::Range;
using ::testing::Values;

static auto triscoreFun = Values( &ComputeTriscoreFromDecodedMsg );

static bool alwaysTrue() { return true; }

INSTANTIATE_TEST_CASE_P(
    TriscoreNoInterleave,
    TriscoreTestSuite,
    Combine(
        Values( &alwaysTrue ),
        triscoreFun )
);

INSTANTIATE_TEST_CASE_P(
    TriscoreNoInterleave,
    LengthTriscoreTestSuite,
    Combine(
        Values( &alwaysTrue ),
        triscoreFun,
        Range<scoreLength_t>( 3, CT ) )
);
