#pragma once


#include "DictsPolicy.hpp"
#include "ExpectedKey.hpp"
#include "HillclimbAssertions.hpp"


extern "C" {
#include "enigma/test_data.h"
}


using Ve3neaAv1DictsPolicy = Ve3neaDictsPolicy< trigraph_AVv1, unigraph_AVv1 >;


namespace heavy_test {
const ExpectedRings ringsVe3Av1 = {
    .ukw = 'B'_ukw,
    .slot = "532"_ringTypes,
};
} // namespace heavy_test


template<>
const heavy_test::ExpectedRings
    HillclimbAssertions< Ve3neaAv1DictsPolicy >::
        expected_rings{heavy_test::ringsVe3Av1};


template<>
void HillclimbAssertions< Ve3neaAv1DictsPolicy >::RunFinalAssertions( const Key& gkey ) const {
    ASSERT_EQ( new_best_cnt, 15 );
    const heavy_test::ExpectedKeyState xkey{
        .score = 59791,
        .stecker = "AGBYCZEJFVKNLPOT",
        .ring = "AAE",
        .mesg = "VVK"};
    AssertMatch( gkey, xkey, expected_rings );
}
