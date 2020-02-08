#include <gtest/gtest.h>


extern "C" {
#include "cipher.h"
#include "input.h"
#include "key.h"
}


TEST( scrambler_state, B_532_AA_AVA_left_wheel_turns_ONSTART ) {
    Key k = {};
    set_key( &k, "B:532:AA:AVA", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 1 );
    EXPECT_EQ( actual.mode, SwMode::SW_ONSTART );
}


TEST( scrambler_state, B_532_AA_AUE_left_wheel_turns_at_step_2 ) {
    Key k = {};
    set_key( &k, "B:532:AA:AUE", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 1 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 2 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_532_AA_AUA_left_wheel_turns_at_step_6 ) {
    Key k = {};
    set_key( &k, "B:532:AA:AUA", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 5 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 6 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_532_AA_ATA_left_wheel_turns_at_step_32 ) {
    Key k = {};
    set_key( &k, "B:532:AA:ATA", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 31 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 32 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_532_AA_APA_left_wheel_turns_at_step_136 ) {
    Key k = {};
    set_key( &k, "B:532:AA:APA", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 135 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 136 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_532_AA_ALA_left_wheel_turns_at_step_240 ) {
    Key k = {};
    set_key( &k, "B:532:AA:ALA", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 239 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 240 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_532_AA_ALQ_left_wheel_turns_at_step_250 ) {
    Key k = {};
    set_key( &k, "B:532:AA:ALQ", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 249 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 250 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_532_AA_AWF_left_wheel_does_not_turn_within_CT_limit ) {
    Key k = {};
    set_key( &k, "B:532:AA:AWF", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 650 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 651 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}


TEST( scrambler_state, B_536_AA_AWA_left_wheel_does_not_turn_within_CT_limit ) {
    Key k = {};
    set_key( &k, "B:536:AA:AWA", EnigmaModel_M3, false );
    SwMode actual = scrambler_state( &k, 325 );
    EXPECT_EQ( actual.mode, SwMode::SW_NONE );
    actual = scrambler_state( &k, 326 );
    EXPECT_EQ( actual.mode, SwMode::SW_OTHER );
}
