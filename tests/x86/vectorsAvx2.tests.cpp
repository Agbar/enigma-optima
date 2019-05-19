#pragma GCC target("avx2")

#include <iomanip>
#include <ios>

#include <gtest/gtest.h>

extern "C" {
#include "x86/vectorsAvx2.h"
}

TEST( m256_setmask_epi32, 0xFF ){
    __v8si result = (__v8si)m256_setmask_epi32( 0xFF );
    for(size_t i =0; i < 8; ++i){
        EXPECT_LT( result[i], 0 )
            << "result[" << i << "] was: " << std::hex << std::setfill('0') << std::setw(8) << std::showbase << result[i];
    }
}

TEST( m256_setmask_epi32, 0b10101010 ){
    __v8si result = (__v8si)m256_setmask_epi32( 0b10101010 );
    for(size_t i =0; i < 4; i += 2){
        EXPECT_GE( result[i], 0 )
            << "result[" << i << "] was: " << std::hex << std::setfill('0') << std::setw(8) << std::showbase << result[i];
        EXPECT_LT( result[i + 1], 0 )
            << "result[" << i << "] was: " << std::hex << std::setfill('0') << std::setw(8) << std::showbase << result[i];
    }
}
