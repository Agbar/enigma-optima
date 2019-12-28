#include <gtest/gtest.h>

extern "C" {
#include "input.h"
}

TEST( set_stecker, loads_example )
{
    Key k = {};
    Fill0To25_echar( k.stbrett.letters );
    char stin[] = "AXBZCNDPEOJMLSQV";
    int result = set_stecker( &k, stin );

    ASSERT_EQ( result, 1 );

    auto letters_swapped = [&k]( char l, char r ) {
        if( k.stbrett.letters[ l - 'A' ].encoded != r - 'A' ) return false;
        if( k.stbrett.letters[ r - 'A' ].encoded != l - 'A' ) return false;
        return true;
    };
    ASSERT_PRED2( letters_swapped, 'A', 'X' );
    ASSERT_PRED2( letters_swapped, 'B', 'Z' );
    ASSERT_PRED2( letters_swapped, 'C', 'N' );
    ASSERT_PRED2( letters_swapped, 'D', 'P' );
    ASSERT_PRED2( letters_swapped, 'E', 'O' );
    ASSERT_PRED2( letters_swapped, 'J', 'M' );
    ASSERT_PRED2( letters_swapped, 'L', 'S' );
    ASSERT_PRED2( letters_swapped, 'Q', 'V' );

    auto letter_not_swapped = [&k]( char c ) {
        return k.stbrett.letters[ c - 'A' ].encoded == c - 'A';
    };
    ASSERT_PRED1( letter_not_swapped, 'F' );
    ASSERT_PRED1( letter_not_swapped, 'G' );
    ASSERT_PRED1( letter_not_swapped, 'H' );
    ASSERT_PRED1( letter_not_swapped, 'I' );
    ASSERT_PRED1( letter_not_swapped, 'K' );
    ASSERT_PRED1( letter_not_swapped, 'R' );
    ASSERT_PRED1( letter_not_swapped, 'T' );
    ASSERT_PRED1( letter_not_swapped, 'U' );
    ASSERT_PRED1( letter_not_swapped, 'W' );
    ASSERT_PRED1( letter_not_swapped, 'Y' );
}


TEST( set_stecker, fails_if_input_over_26_chars )
{
    Key k = {};
    Fill0To25_echar( k.stbrett.letters );
    char stin[] = "ABCDEFGHIJKLMNOPRSTUVWXYZ01";
    int result = set_stecker( &k, stin );
    ASSERT_FALSE( result );
}


TEST( set_stecker, fails_if_input_count_is_odd )
{
    Key k = {};
    Fill0To25_echar( k.stbrett.letters );
    char stin[] = "ABC";
    int result = set_stecker( &k, stin );
    ASSERT_FALSE( result );
}


TEST( set_stecker, fails_if_input_contains_not_letter )
{
    Key k = {};
    Fill0To25_echar( k.stbrett.letters );
    char stin[] = "ABC1";
    int result = set_stecker( &k, stin );
    ASSERT_FALSE( result );
}


TEST( set_stecker, fails_if_input_contains_repetition )
{
    Key k = {};
    Fill0To25_echar( k.stbrett.letters );
    char stin[] = "ABCA";
    int result = set_stecker( &k, stin );
    ASSERT_FALSE( result );
}
