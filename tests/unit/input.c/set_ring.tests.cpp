#include <gtest/gtest.h>

extern "C" {
#include "input.h"
}


TEST( set_ring_H_M3, loads_example )
{
    Key key;
    // invalid
    key.ring = RingsState{ 
        .g = { UINT8_MAX },
        .l = { UINT8_MAX },
        .m = { UINT8_MAX }, 
        .r = { UINT8_MAX } };
    char s[] = "ABC";

    int result = set_ring( &key, s, ModelType_t::EnigmaModel_H );
    ASSERT_TRUE( result );

    ASSERT_EQ( key.ring.l.delta, 0 );
    ASSERT_EQ( key.ring.m.delta, 1 );
    ASSERT_EQ( key.ring.r.delta, 2 );
}


TEST( set_ring_M4, loads_example )
{
    Key key;
    // invalid
    key.ring = RingsState{ 
        .g = { UINT8_MAX },
        .l = { UINT8_MAX },
        .m = { UINT8_MAX }, 
        .r = { UINT8_MAX } };
    char s[] = "ABCD";

    int result = set_ring( &key, s, ModelType_t::EnigmaModel_M4 );
    ASSERT_TRUE( result );

    ASSERT_EQ( key.ring.g.delta, 0 );
    ASSERT_EQ( key.ring.l.delta, 1 );
    ASSERT_EQ( key.ring.m.delta, 2 );
    ASSERT_EQ( key.ring.r.delta, 3 );
}


TEST( set_ring_M4, fails_when_only_3_chars )
{
    Key key;
    // invalid
    key.ring = RingsState{ 
        .g = { UINT8_MAX },
        .l = { UINT8_MAX },
        .m = { UINT8_MAX }, 
        .r = { UINT8_MAX } };
    char s[] = "ABC";

    int result = set_ring( &key, s, ModelType_t::EnigmaModel_M4 );
    ASSERT_FALSE( result );
}


TEST( set_ring_H_M3, fails_when_4_chars )
{
    Key key;
    // invalid
    key.ring = RingsState{ 
        .g = { UINT8_MAX },
        .l = { UINT8_MAX },
        .m = { UINT8_MAX }, 
        .r = { UINT8_MAX } };
    char s[] = "ABCD";

    int result = set_ring( &key, s, ModelType_t::EnigmaModel_H );
    ASSERT_FALSE( result );
}


TEST( set_ring, fails_when_not_letter )
{
    init_charmap();
    Key key;
    // invalid
    key.ring = RingsState{ 
        .g = { UINT8_MAX },
        .l = { UINT8_MAX },
        .m = { UINT8_MAX }, 
        .r = { UINT8_MAX } };
    char s[] = "AB0";

    int result = set_ring( &key, s, ModelType_t::EnigmaModel_H );
    ASSERT_FALSE( result );
}
