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

TEST( char_delta, invert )
{
    struct echar_delta x;
    x.delta = 0;
    EXPECT_EQ( 0, echar_delta_invert( x ).delta );
    x.delta = 25;
    EXPECT_EQ( 1, echar_delta_invert( x ).delta );
    x.delta = 1;
    EXPECT_EQ( 25, echar_delta_invert( x ).delta );
}