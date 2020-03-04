#include <gtest/gtest.h>

extern "C" {
#include "key.h"
}


class Key_equ_test
    : public ::testing::Test {
protected:
    Key m4{
        stbrett : {},
        mesg : {
            r : make_echar_delta_ascii( 'I' ),
            m : make_echar_delta_ascii( 'U' ),
            l : make_echar_delta_ascii( 'Y' ),
            g : make_echar_delta_ascii( 'T' ),
        },
        slot : {g : {GreekRingType_Beta},
                l : {RingType_3},
                m : {RingType_8},
                r : {RingType_6}},
        ring : {m : make_echar_delta_ascii( 'X' ),
                r : make_echar_delta_ascii( 'H' )},
        ukwnum : {UkwType_B_Thin},
        model : EnigmaModel_M3,
        sf : {},
        count : 0,
        score : 0

    };
};


TEST_F( Key_equ_test, key_is_equal_to_itself ) {
    ASSERT_TRUE( Key_equ( &m4, &m4 ) );
}


TEST_F( Key_equ_test, identical_keys_are_equal ) {
    Key copy = m4;
    ASSERT_TRUE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_mesg_g ) {
    Key copy = m4;
    copy.mesg.g = make_echar_delta_ascii( 'L' );
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_mesg_l ) {
    Key copy = m4;
    copy.mesg.l = make_echar_delta_ascii( 'A' );
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_mesg_m ) {
    Key copy = m4;
    copy.mesg.m = make_echar_delta_ascii( 'S' );
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_mesg_r ) {
    Key copy = m4;
    copy.mesg.r = make_echar_delta_ascii( 'D' );
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_ringstellung_m ) {
    Key copy = m4;
    copy.ring.m = make_echar_delta_ascii( 'D' );
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_ringstellung_r ) {
    Key copy = m4;
    copy.ring.r = make_echar_delta_ascii( 'E' );
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_slot_g ) {
    Key copy = m4;
    copy.slot.g = {GreekRingType_Gamma};
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_slot_l ) {
    Key copy = m4;
    copy.slot.l = {RingType_2};
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_slot_m ) {
    Key copy = m4;
    copy.slot.m = {RingType_4};
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_slot_r ) {
    Key copy = m4;
    copy.slot.r = {RingType_7};
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}


TEST_F( Key_equ_test, difference_in_ukw ) {
    Key copy = m4;
    copy.ukwnum = {UkwType_C_Thin};
    ASSERT_FALSE( Key_equ( &m4, &copy ) );
}
