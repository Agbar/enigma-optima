#include <gtest/gtest.h>
extern "C" {
    #include "../OS/Os.h"
}

// There is tiny tiny chance that this test will fail. However chances should
// be around one in 4 billions.
TEST( GetRSeedTest, ReturnsDifferentValues )
{
    unsigned int first  = GetRSeed();
    unsigned int second = GetRSeed();

    EXPECT_NE( first, second );
}
