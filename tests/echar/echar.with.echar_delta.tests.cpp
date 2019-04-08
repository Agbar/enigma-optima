#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar.h"
    #include "echar/echar_delta.h"
}

TEST( echar__echar_delta , echar_sub_delta_a ){
    auto a = make_echar( 0 );
    echar_delta diff;
    diff.delta = 0;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 0 );
    diff.delta = 1;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 26 - 1 );
    diff.delta = 9;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 26 - 9 );
    diff.delta = 10;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 26 - 10 );
    diff.delta = 15;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 26 - 15 );
    diff.delta = 16;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 26 - 16 );
    diff.delta = 25;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 26 - 25 );
}

TEST( echar__echar_delta , echar_sub_delta_z ){
    auto a = make_echar( 25 );
    echar_delta diff;
    diff.delta = 0;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 );
    diff.delta = 1;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 - 1 );
    diff.delta = 9;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 - 9 );
    diff.delta = 10;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 - 10 );
    diff.delta = 15;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 - 15 );
    diff.delta = 16;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 - 16 );
    diff.delta = 25;
    ASSERT_EQ( echar_sub_delta( a, diff ).encoded, 25 - 25 );
}