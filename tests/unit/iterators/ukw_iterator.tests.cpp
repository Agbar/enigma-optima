#include <gtest/gtest.h>


extern "C" {
#include "echar/echar_delta.h"
#include "iterators/ukw_iterator.h"
}


static bool operator==( const UkwType lhs, const UkwType rhs ) {
    return UkwType_equ( lhs, rhs );
}


TEST( ukw_iterator_h, next_after_A_is_B ) {
    UkwIterator iter{
        ukw : {UkwType_A},
        overflow : false,
        model : EnigmaModel_H,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_B} );
    EXPECT_FALSE( nxt.overflow );
    ASSERT_FALSE( UkwIterator_equ( nxt, ukw_overflow() ) );
}


TEST( ukw_iterator_h, next_after_B_is_C ) {
    UkwIterator iter{
        ukw : {UkwType_B},
        overflow : false,
        model : EnigmaModel_H,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_C} );
    EXPECT_FALSE( nxt.overflow );
    ASSERT_FALSE( UkwIterator_equ( nxt, ukw_overflow() ) );
}


TEST( ukw_iterator_h, next_after_C_is_A_with_overflow ) {
    UkwIterator iter{
        ukw : {UkwType_C},
        overflow : false,
        model : EnigmaModel_H,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_A} );
    ASSERT_TRUE( nxt.overflow );
    ASSERT_TRUE( UkwIterator_equ( nxt, ukw_overflow() ) );
}


TEST( ukw_iterator_m3, next_after_B_is_C ) {
    UkwIterator iter{
        ukw : {UkwType_B},
        overflow : false,
        model : EnigmaModel_M3,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_C} );
    EXPECT_FALSE( nxt.overflow );
    ASSERT_FALSE( UkwIterator_equ( nxt, ukw_overflow() ) );
}


TEST( ukw_iterator_m3, next_after_C_is_B_with_overflow ) {
    UkwIterator iter{
        ukw : {UkwType_C},
        overflow : false,
        model : EnigmaModel_M3,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_B} );
    ASSERT_TRUE( nxt.overflow );
    ASSERT_TRUE( UkwIterator_equ( nxt, ukw_overflow() ) );
}


TEST( ukw_iterator_m4, next_after_B_is_C ) {
    UkwIterator iter{
        ukw : {UkwType_B_Thin},
        overflow : false,
        model : EnigmaModel_M4,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_C_Thin} );
    EXPECT_FALSE( nxt.overflow );
    ASSERT_FALSE( UkwIterator_equ( nxt, ukw_overflow() ) );
}


TEST( ukw_iterator_m4, next_after_C_is_B_with_overflow ) {
    UkwIterator iter{
        ukw : {UkwType_C_Thin},
        overflow : false,
        model : EnigmaModel_M4,
    };
    UkwIterator nxt = next_ukw( iter );

    ASSERT_EQ( nxt.ukw, UkwType{UkwType_B_Thin} );
    ASSERT_TRUE( nxt.overflow );
    ASSERT_TRUE( UkwIterator_equ( nxt, ukw_overflow() ) );
}
