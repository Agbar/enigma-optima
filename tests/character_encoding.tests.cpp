#include <gtest/gtest.h>

extern "C" {
    #include "character_encoding.h"
}

TEST( echar_delta, sub) 
{   
    const echar_delta e0 { 0 };
    const echar_delta e1 { 1 };
    const echar_delta e25 { 25 };

    EXPECT_EQ( 25, echar_delta_sub(  e0,  e1 ).delta );
    EXPECT_EQ(  0, echar_delta_sub( e25, e25 ).delta );
    EXPECT_EQ(  0, echar_delta_sub(  e0,  e0 ).delta );
    EXPECT_EQ(  1, echar_delta_sub(  e0, e25 ).delta );
    EXPECT_EQ( 25, echar_delta_sub( e25,  e0 ).delta );
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