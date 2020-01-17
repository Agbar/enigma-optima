#pragma once


#include <gtest/gtest.h>


#include "DictsPolicy.hpp"
#include "ExpectedKey.hpp"
#include "HillclimbAssertions.hpp"


extern "C" {
#include "enigma/test_data.h"
}


using Krah1941DictsPolicy = KrahDictsPolicy< trigraph_1941, bigraph_1941 >;


namespace heavy_test {
const ExpectedRings rings1941 = {
    .ukw = 'B'_ukw,
    .slot = "532"_ringTypes,
};
} // namespace heavy_test


template<>
const heavy_test::ExpectedRings
    HillclimbAssertions< Krah1941DictsPolicy >::
        expected_rings{heavy_test::rings1941};


template<>
void HillclimbAssertions< Krah1941DictsPolicy >::RunFinalAssertions( const Key& gkey ) const {
    ASSERT_EQ( new_best_cnt, 12 );
    const heavy_test::ExpectedKeyState xkey{
        .score = 17930,
        .stecker = "AIBECJDRFYGOHZMUNQPVST",
        .ring = "AAC",
        .mesg = "HVS"};
    AssertMatch( gkey, xkey, expected_rings );
}


template<>
void HillclimbAssertions< Krah1941DictsPolicy >::OnNewBest( UNUSED const Key& gkey ) {
    new_best_cnt++;
}
