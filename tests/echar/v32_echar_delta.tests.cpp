#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar_delta.h"
}

TEST( v32_echar_delta, rot_32 ){
    v32_echar_delta v1;
    v1.vector = (v32qi){ 0,  1,  9, 10, 15, 16, 24, 25,
                         2,  3,  4,  5,  6,  7,  8, 23,
                        22, 21, 20, 19, 18, 17, 14, 13,
                        12, 11,  8,  7,  3,  3,  7,  1 };
    v32qi expected =   { 6,  7, 15, 16, 21, 22,  4,  5,
                         8,  9, 10, 11, 12, 13, 14,  3,
                         2,  1,  0, 25, 24, 23, 20, 19,
                        18, 17, 14, 13,  9,  9, 13,  7 };
    
    auto rotated = v32_echar_delta_rot_32( v1 );

    for( size_t i = 0; i < sizeof(v1); ++i ){
        EXPECT_EQ( rotated.vector[i] , expected[i] ) << "Vectors differ at index " << i;
    }
}