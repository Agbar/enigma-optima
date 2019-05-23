#include <gtest/gtest.h>

#include "computeScore.tests.hpp"


extern "C"{
    #include "dict.h"
}

TEST_P( TriscoreTestSuite, Len0To2Scores0 ){
     tridict[0][0][0] = 1;

    ASSERT_EQ( ComputeTriscore( 0 ), 0 );
    ASSERT_EQ( ComputeTriscore( 1 ), 0 );
    ASSERT_EQ( ComputeTriscore( 2 ), 0 );
}

TEST_P( TriscoreTestSuite, len_3 ) {
    Message().plain[0] = make_echar( 1 );
    Message().plain[1] = make_echar( 2 );
    Message().plain[2] = make_echar( 3 );
    
    tridict[1][2][3] = 42;

    ASSERT_EQ( ComputeTriscore( 3 ), 42 );
}


TEST_P( LengthTriscoreTestSuite, AddsAllTriplets ){
    tridict[0][0][0] = 1;
    ASSERT_EQ( ComputeTriscore(), MsgLength() - 2 );
}

