#include <gtest/gtest.h>

extern "C" {
#include "input.h"
}


TEST( set_RingsState_H_M3, loads_example )
{
    // invalid
    RingsState rs{
        .r = {UINT8_MAX},
        .m = {UINT8_MAX},
        .l = {UINT8_MAX},
        .g = {UINT8_MAX},
    };
    char s[] = "ABC";

    int result = set_RingsState( &rs, s, ModelType_t::EnigmaModel_H );
    ASSERT_TRUE( result );

    ASSERT_EQ( rs.l.delta, 0 );
    ASSERT_EQ( rs.m.delta, 1 );
    ASSERT_EQ( rs.r.delta, 2 );
}


TEST( set_RingsState_M4, loads_example )
{
    // invalid
    RingsState rs{
        .r = {UINT8_MAX},
        .m = {UINT8_MAX},
        .l = {UINT8_MAX},
        .g = {UINT8_MAX},
    };
    char s[] = "ABCD";

    int result = set_RingsState( &rs, s, ModelType_t::EnigmaModel_M4 );
    ASSERT_TRUE( result );

    ASSERT_EQ( rs.g.delta, 0 );
    ASSERT_EQ( rs.l.delta, 1 );
    ASSERT_EQ( rs.m.delta, 2 );
    ASSERT_EQ( rs.r.delta, 3 );
}


TEST( set_RingsState_M4, fails_when_only_3_chars )
{
    // invalid
    RingsState rs{
        .r = {UINT8_MAX},
        .m = {UINT8_MAX},
        .l = {UINT8_MAX},
        .g = {UINT8_MAX},
    };
    char s[] = "ABC";

    int result = set_RingsState( &rs, s, ModelType_t::EnigmaModel_M4 );
    ASSERT_FALSE( result );
}


TEST( set_RingsState_H_M3, fails_when_4_chars )
{
    // invalid
    RingsState rs{
        .r = {UINT8_MAX},
        .m = {UINT8_MAX},
        .l = {UINT8_MAX},
        .g = {UINT8_MAX},
    };
    char s[] = "ABCD";

    int result = set_RingsState( &rs, s, ModelType_t::EnigmaModel_H );
    ASSERT_FALSE( result );
}


TEST( set_RingsState, fails_when_not_letter )
{
    // invalid
    RingsState rs{
        .r = {UINT8_MAX},
        .m = {UINT8_MAX},
        .l = {UINT8_MAX},
        .g = {UINT8_MAX},
    };
    char s[] = "AB0";

    int result = set_RingsState( &rs, s, ModelType_t::EnigmaModel_H );
    ASSERT_FALSE( result );
}
