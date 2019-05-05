#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar.h"
    #include "echar/echar_delta.h"

}

TEST( v16_echar__v16_echar_delta, v16_echar_add_delta ){
    v16_echar base {};
    base.vector += 13;
    v16_echar_delta offset;
    offset.vector = (v16qu){ 20, 19, 18, 17, 16, 15, 14, 13,
                              5,  6,  7,  8,  9, 10, 11, 12 };

    v16_echar sum = v16_echar_add_delta( base, offset );

    const v16qs expected = {  7,  6,  5,  4,  3,  2,  1,  0,
                             18, 19, 20, 21, 22, 23, 24, 25 };
    for( size_t i = 0; i < sizeof(sum.vector); ++i ){
        EXPECT_EQ( sum.vector[i], expected[i] ) << "Vectors differ at index " << i;
    }
}

TEST( v16_echar__v16_echar_delta, v16_echar_sub_delta ){
    v16_echar base {};
    base.vector += 13;
    v16_echar_delta offset;
    offset.vector = (v16qu){ 20, 19, 18, 17, 16, 15, 14, 13,
                              5,  6,  7,  8,  9, 10, 11, 12 };

    v16_echar sum = v16_echar_sub_delta( base, offset );

    const v16qs expected = { 19, 20, 21, 22, 23, 24, 25, 0,
                              8,  7,  6,  5,  4,  3,  2, 1 };
    for( size_t i = 0; i < sizeof(sum.vector); ++i ){
        EXPECT_EQ( sum.vector[i], expected[i] ) << "Vectors differ at index " << i;
    }
}
