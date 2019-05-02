#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar.h"
    #include "echar/echar_delta.h"
}

TEST( v32_echar__v32_echar_delta, v32_echar_add_delta ){
    v32_echar base {};
    base.vector += 13;
    v32_echar_delta offset;
    offset.vector = (v32qi){ 20, 19, 18, 17, 16, 15, 14, 13,
                              5,  6,  7,  8,  9, 10, 11, 12,
                              1,  1,  1,  1,  0,  1,  1,  1,
                              1,  1,  1,  1,  1,  1,  1,  1 };

    v32_echar sum = v32_echar_add_delta( base, offset );

    const v32qi expected = {  7,  6,  5,  4,  3,  2,  1,  0,
                             18, 19, 20, 21, 22, 23, 24, 25,
                             14, 14, 14, 14, 13, 14, 14, 14,
                             14, 14, 14, 14, 14, 14, 14, 14 };
    for( size_t i = 0; i < sizeof(sum.vector); ++i ){
        EXPECT_EQ( sum.vector[i], expected[i] ) << "Vectors differ at index " << i;
    }
}

TEST( v32_echar__v32_echar_delta, v32_echar_sub_delta ){
    v32_echar base {};
    base.vector += 13;
    v32_echar_delta offset;
    offset.vector = (v32qi){ 20, 19, 18, 17, 16, 15, 14, 13,
                              5,  6,  7,  8,  9, 10, 11, 12,
                              1,  1,  1,  0,  1,  1,  1,  1,
                              1,  1,  1,  1,  1,  1,  1,  1 };

    v32_echar sum = v32_echar_sub_delta( base, offset );

    const v32qi expected = { 19, 20, 21, 22, 23, 24, 25,  0,
                              8,  7,  6,  5,  4,  3,  2,  1,
                             12, 12, 12, 13, 12, 12, 12, 12,
                             12, 12, 12, 12, 12, 12, 12, 12 };
    for( size_t i = 0; i < sizeof(sum.vector); ++i ){
        EXPECT_EQ( sum.vector[i], expected[i] ) << "Vectors differ at index " << i;
    }
}