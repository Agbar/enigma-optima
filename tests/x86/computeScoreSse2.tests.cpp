#include <cstring>
#include <cstdint>
#include <gtest/gtest.h>

extern "C"{
    #include "dict.h"
    #include "x86/computeScoreSse2.h"
}

template< size_t last_dim_size, size_t dim_size >
void reset_tridict( dict_t (&td)[last_dim_size][dim_size][dim_size] ){
    std::memset( &td, 0, sizeof td );
}

TEST( ComputeTriscoreFromDecodedMsgSse2Tests, len_0_1_2 ) {
    ScoringDecodedMessage msg {};
    reset_tridict( tridict );
    tridict[0][0][0] = 1;

    ASSERT_EQ( ComputeTriscoreFromDecodedMsgSse2( &msg, 0 ), 0 );
    ASSERT_EQ( ComputeTriscoreFromDecodedMsgSse2( &msg, 1 ), 0 );
    ASSERT_EQ( ComputeTriscoreFromDecodedMsgSse2( &msg, 2 ), 0 );
}

TEST( ComputeTriscoreFromDecodedMsgSse2Tests, len_3 ) {
    ScoringDecodedMessage msg { .plain = { make_echar( 1 ), make_echar( 2 ), make_echar( 3 ) } };
    reset_tridict( tridict );
    tridict[1][2][3] = 42;

    ASSERT_EQ( ComputeTriscoreFromDecodedMsgSse2( &msg, 3 ), 42 );
}

class LengthSse2:
    public ::testing::TestWithParam<uint8_t> {
public:
    ScoringDecodedMessage msg {};

    static void SetUpTestCase(){
        reset_tridict( tridict );
        tridict[0][0][0] = 1;
    }
};

TEST_P( LengthSse2, AddsAllTriplets ){
    ASSERT_EQ( ComputeTriscoreFromDecodedMsgSse2( &msg, GetParam() ), GetParam() - 2 );
}

INSTANTIATE_TEST_CASE_P(
    ComputeTriscoreFromDecodedMsgSse2Tests, 
    LengthSse2,
    ::testing::Range<uint8_t>( 3, CT ) );
