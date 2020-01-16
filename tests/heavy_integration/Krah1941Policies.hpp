#pragma once


#include <gtest/gtest.h>


#include "DictsPolicy.hpp"
#include "ExpectedKey.hpp"
#include "HillclimbAssertions.hpp"


extern "C" {
#include "enigma/test_data.h"
}


using Krah1941DictsPolicy = KrahDictsPolicy< trigraph_1941, bigraph_1941 >;


template<>
void HillclimbAssertions< Krah1941DictsPolicy >::RunFinalAssertions( const Key& gkey ) const {
    ASSERT_EQ( new_best_cnt, 12 );

    using namespace heavy_test;
    const heavy_test::ExpectedKey expected = {
        .score = 17930,
        .ukw = {.type = UkwType::UkwType_B},
        .slot = "532"_ringTypes,
        .stecker = "AIBECJDRFYGOHZMUNQPVST",
        .ring = "AAC",
        .mesg = "HVS"};

    expected.AssertMatch( gkey );
}


template<>
void HillclimbAssertions< Krah1941DictsPolicy >::OnNewBest( UNUSED const Key& gkey ) {
    new_best_cnt++;
}
