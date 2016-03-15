#include <gtest/gtest.h>
#include "../cipher.h"

TEST( Step1Test , Changes_N_into_N_plus_1 )
{
    int8_t state = 0;

    for( int8_t expected = 1; expected <= 25; ++expected)
    {
        Step1( &state );
        EXPECT_EQ( expected,  state );
    }
}

TEST ( Step1Test, Changes_25_into_0 )
{
    int8_t state = 25;
    Step1 ( &state );
    EXPECT_EQ( 0, state );
}
