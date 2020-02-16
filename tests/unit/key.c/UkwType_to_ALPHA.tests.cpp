#include <gtest/gtest.h>

extern "C" {
#include "key.h"
}


TEST( UkwType_to_ALPHA, A ) {
    UkwType u{UkwType_A};
    ASSERT_EQ( UkwType_to_ALPHA( u ), 'A' );
}


TEST( UkwType_to_ALPHA, B ) {
    UkwType u{UkwType_B};
    ASSERT_EQ( UkwType_to_ALPHA( u ), 'B' );
}


TEST( UkwType_to_ALPHA, C ) {
    UkwType u{UkwType_C};
    ASSERT_EQ( UkwType_to_ALPHA( u ), 'C' );
}


TEST( UkwType_to_ALPHA, thin_B ) {
    UkwType u{UkwType_B_Thin};
    ASSERT_EQ( UkwType_to_ALPHA( u ), 'B' );
}


TEST( UkwType_to_ALPHA, thin_C ) {
    UkwType u{UkwType_C_Thin};
    ASSERT_EQ( UkwType_to_ALPHA( u ), 'C' );
}
