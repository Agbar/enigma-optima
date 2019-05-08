#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar_delta.h"
}

TEST( v16_echar_delta, rot_16 ){
    v16_echar_delta v1;
    v1.vector = (v16qu){  0,  1,  9, 10, 15, 16, 24, 25,  2,  3,  4,  5,  6,  7,  8, 11 };
    v16qu expected =   { 16, 17, 25,  0,  5,  6, 14, 15, 18, 19, 20, 21, 22, 23, 24,  1 };
    auto rotated = v16_echar_delta_rot_16( v1 );
    for( size_t i = 0; i < sizeof(v1); ++i ){
        ASSERT_EQ( rotated.vector[i] , expected[i] ) << "Vectors differ at index " << i;
    }
}