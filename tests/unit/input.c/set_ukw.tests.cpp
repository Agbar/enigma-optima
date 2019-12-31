#include <gtest/gtest.h>

extern "C" {
#include "input.h"
}


TEST( set_ukw, H_M3_ukw_A ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "A", ModelType_t::EnigmaModel_H ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_A );
}


TEST( set_ukw, H_M3_ukw_a ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "a", ModelType_t::EnigmaModel_H ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_A );
}


TEST( set_ukw, H_M3_ukw_B ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "B", ModelType_t::EnigmaModel_H ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_B );
}


TEST( set_ukw, H_M3_ukw_b ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "b", ModelType_t::EnigmaModel_H ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_B );
}


TEST( set_ukw, H_M3_ukw_C ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "C", ModelType_t::EnigmaModel_H ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_C );
}


TEST( set_ukw, H_M3_ukw_c ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "c", ModelType_t::EnigmaModel_H ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_C );
}


TEST( set_ukw, H_M3_fails_with_other ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_FALSE( set_ukw( &k, "d", ModelType_t::EnigmaModel_H ) );
}


TEST( sek_ukw, M4_ukw_A_fails ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_FALSE( set_ukw( &k, "A", ModelType_t::EnigmaModel_M4 ) );
    EXPECT_FALSE( set_ukw( &k, "a", ModelType_t::EnigmaModel_M4 ) );
}


TEST( sek_ukw, M4_ukw_B ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "B", ModelType_t::EnigmaModel_M4 ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_B_Thin );
}


TEST( sek_ukw, M4_ukw_b ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "b", ModelType_t::EnigmaModel_M4 ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_B_Thin );
}


TEST( sek_ukw, M4_ukw_C ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "C", ModelType_t::EnigmaModel_M4 ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_C_Thin );
}


TEST( sek_ukw, M4_ukw_c ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_TRUE( set_ukw( &k, "c", ModelType_t::EnigmaModel_M4 ) );
    EXPECT_EQ( k.ukwnum.type, UkwType::UkwType_C_Thin );
}


TEST( sek_ukw, M4_ukw_fails_with_other ) {
    Key k;
    k.ukwnum.type = (UkwType::ukw_type_enum)-1;
    EXPECT_FALSE( set_ukw( &k, "d", ModelType_t::EnigmaModel_M4 ) );
}
