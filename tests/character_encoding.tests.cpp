#include <gtest/gtest.h>

extern "C" {
    #include "character_encoding.h"
}

TEST( make_char_delta, plus_minus) 
{
    EXPECT_EQ( 25, make_char_delta_plus_minus(  0,  1 ).delta );
    EXPECT_EQ(  0, make_char_delta_plus_minus( 25, 25 ).delta );
    EXPECT_EQ(  0, make_char_delta_plus_minus(  0,  0 ).delta );
    EXPECT_EQ(  1, make_char_delta_plus_minus(  0, 25 ).delta );
    EXPECT_EQ( 25, make_char_delta_plus_minus(  25, 0 ).delta );
}
