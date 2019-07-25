#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar.h"
}

TEST( echar, make_0_based ){
    auto ec = make_echar( 0 );
    ASSERT_EQ( ec.encoded, 0 );
    ec = make_echar( 25 );
    ASSERT_EQ( ec.encoded, 25 );
    ec = make_echar( 16 );
    ASSERT_EQ( ec.encoded, 16 );
    ec = make_echar( 9 );
    ASSERT_EQ( ec.encoded, 9 );
}

TEST( echar, can_make_from_ascii ){
    init_charmap();
    for( char x = 'a'; x <= 'z'; ++x ){
        ASSERT_TRUE( echar_can_make_from_ascii( x ) );
    }
    for( char x = 'A'; x <= 'Z'; ++x ){
        ASSERT_TRUE( echar_can_make_from_ascii( x ) );
    }
}

TEST( echar, from_ascii ){
    init_charmap();
    ASSERT_EQ( make_echar_ascii( 'a' ).encoded, 0 );
    ASSERT_EQ( make_echar_ascii( 'z' ).encoded, 25 );
}

TEST( echar, to_alpha ){
    echar zero;
    zero.encoded = 0;
    ASSERT_EQ( echar_to_alpha( zero ), 'a' );
    echar twenty_five;
    twenty_five.encoded = 25;
    ASSERT_EQ( echar_to_alpha( twenty_five ), 'z' );
}

TEST( echar, to_ALPHA ){
    echar zero;
    zero.encoded = 0;
    ASSERT_EQ( echar_to_ALPHA( zero ), 'A' );
    echar twenty_five;
    twenty_five.encoded = 25;
    ASSERT_EQ( echar_to_ALPHA( twenty_five ), 'Z' );
}

TEST( echar, 0_based_index ){
    init_charmap();
    echar a = make_echar_ascii( 'a' );
    ASSERT_EQ( echar_0_based_index( a ), 0 );
    echar z = make_echar_ascii( 'z' );
    ASSERT_EQ( echar_0_based_index( z ), 25 );
}

TEST( echar, eq ){
    init_charmap();
    echar p = make_echar_ascii( 'p' );
    ASSERT_TRUE( echar_eq( p, p ) );
    echar q = make_echar_ascii( 'q' );
    ASSERT_FALSE( echar_eq( p, q ) );
}

TEST( echar, neq ){
    init_charmap();
    echar p = make_echar_ascii( 'p' );
    echar q = make_echar_ascii( 'q' );
    ASSERT_TRUE( echar_neq( p, q ) );
    ASSERT_FALSE( echar_neq( p, p ) );
}
