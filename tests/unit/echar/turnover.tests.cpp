#include "gtest/gtest.h"

extern "C" {
    #include "echar/turnover.h"
}

TEST( turnover, eq_absent ){
    turnover should_be_absent;
    should_be_absent.notch = ~0;

    ASSERT_TRUE( turnover_eq_absent( should_be_absent ) );
}
