#include "gtest/gtest.h"

extern "C" {
    #include "cipher.h"
    #include "echar/echar_delta.h"
}

TEST( echar_delta, from_ascii ){
    ASSERT_EQ( make_echar_delta_ascii( 'a' ).delta, 0 );
    ASSERT_EQ( make_echar_delta_ascii( 'z' ).delta, 25 );
    ASSERT_EQ( make_echar_delta_ascii( 'A' ).delta, 0 );
    ASSERT_EQ( make_echar_delta_ascii( 'Z' ).delta, 25 );
}

TEST( echar_delta, rot_1 ){
    echar_delta d;
    {
        d.delta = 0;
        echar_delta_rot_1( &d );
        ASSERT_EQ( d.delta, 1 );
    }
    {
        d.delta = 25;
        echar_delta_rot_1( &d );
        ASSERT_EQ( d.delta, 0 );
    }
}

TEST( echar_delta, rot_13 ){
    echar_delta d;
    {
        d.delta = 0;
        echar_delta_rot_13( &d );
        ASSERT_EQ( d.delta, 13 );
    }
    {
        d.delta = 12;
        echar_delta_rot_13( &d );
        ASSERT_EQ( d.delta, 25 );
    }
    {
        d.delta = 13;
        echar_delta_rot_13( &d );
        ASSERT_EQ( d.delta, 0 );
    }
    {
        d.delta = 25;
        echar_delta_rot_13( &d );
        ASSERT_EQ( d.delta, 12 );
    }
}

TEST( echar_delta, rot_15 ){
    echar_delta d;
    {
        d.delta = 0;
        echar_delta_rot_15( &d );
        ASSERT_EQ( d.delta, 15 );
    }
    {
        d.delta = 10;
        echar_delta_rot_15( &d );
        ASSERT_EQ( d.delta, 25 );
    }
    {
        d.delta = 11;
        echar_delta_rot_15( &d );
        ASSERT_EQ( d.delta, 0 );
    }
    {
        d.delta = 25;
        echar_delta_rot_15( &d );
        ASSERT_EQ( d.delta, 14 );
    }
}

TEST( echar_delta, rot_31 ){
    echar_delta d;
    {
        d.delta = 0;
        echar_delta_rot_31( &d );
        ASSERT_EQ( d.delta, 31 % 26 );
    }
    {
        d.delta = 20;
        echar_delta_rot_31( &d );
        ASSERT_EQ( d.delta, 25 );
    }
    {
        d.delta = 21;
        echar_delta_rot_31( &d );
        ASSERT_EQ( d.delta, 0 );
    }
    {
        d.delta = 25;
        echar_delta_rot_31( &d );
        ASSERT_EQ( d.delta, 4 );
    }
}

TEST( echar_delta, cmp ){
    echar_delta zero { 0 };
    echar_delta one  { 1 };
    echar_delta two  { 2 };
    ASSERT_EQ( echar_delta_cmp( one, one ), cmp_equal );
    ASSERT_EQ( echar_delta_cmp( one, two ), cmp_less );
    ASSERT_EQ( echar_delta_cmp( two, zero ), cmp_greater );
}

TEST( echar_delta, sub ){
    echar_delta zero { 0 };
    echar_delta one  { 1 };
    echar_delta ten  { 10 };
    echar_delta twenty_five { 25 };
    {
        ASSERT_EQ( echar_delta_sub( ten, zero ).delta, 10 );
        ASSERT_EQ( echar_delta_sub( ten, one ).delta, 9 );
    }
    {
        ASSERT_EQ( echar_delta_sub( zero, zero ).delta, 0 );
        ASSERT_EQ( echar_delta_sub( zero, one ).delta, 25 );
        ASSERT_EQ( echar_delta_sub( zero, ten ).delta, 16 );
        ASSERT_EQ( echar_delta_sub( zero, twenty_five ).delta, 1 );
    }
    {
        ASSERT_EQ( echar_delta_sub( one, one ).delta, 0 );
        ASSERT_EQ( echar_delta_sub( ten, ten ).delta, 0 );
        ASSERT_EQ( echar_delta_sub( twenty_five, twenty_five ).delta, 0 );
    }
    {
        ASSERT_EQ( echar_delta_sub( twenty_five, one ).delta, 24 );
    }
}

TEST( echar_delta, invert )
{
    struct echar_delta x;
    x.delta = 0;
    EXPECT_EQ( 0, echar_delta_invert( x ).delta );
    x.delta = 25;
    EXPECT_EQ( 1, echar_delta_invert( x ).delta );
    x.delta = 1;
    EXPECT_EQ( 25, echar_delta_invert( x ).delta );
}

TEST( echar_delta, to_ALPHA ){
    echar_delta d;
    d.delta = 0;
    ASSERT_EQ( echar_delta_to_ALPHA( d ), 'A' );
    d.delta = 25;
    ASSERT_EQ( echar_delta_to_ALPHA( d ), 'Z' );
}
