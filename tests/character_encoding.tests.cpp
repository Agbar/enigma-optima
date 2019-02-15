#include <gtest/gtest.h>

extern "C" {
    #include "character_encoding.h"
}

TEST( character_encoding, enigma_character_size )
{
    EXPECT_EQ( 1, sizeof(enigma_character) );
}

TEST( character_encoding, v16_echar_size )
{
    EXPECT_EQ( 16, sizeof(v16_echar) );
}

TEST( character_encoding, v32_echar_size )
{
    EXPECT_EQ( 32, sizeof(v32_echar) );
}

TEST( character_encoding, enigma_char_delta_size )
{
    EXPECT_EQ( 1, sizeof(enigma_char_delta) );
}

TEST( character_encoding, v16_echar_delta_size )
{
    EXPECT_EQ( 16, sizeof(v16_echar_delta) );
}

TEST( character_encoding, v32_echar_delta_size )
{
    EXPECT_EQ( 32, sizeof(v32_echar_delta) );
}
