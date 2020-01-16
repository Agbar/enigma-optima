#pragma once


#include "DictsPolicy.hpp"
#include "ExpectedKey.hpp"
#include "HillclimbAssertions.hpp"


extern "C" {
#include "enigma/test_data.h"
}


using Krah1941DictsPolicy = KrahDictsPolicy< trigraph_1941, bigraph_1941 >;


template<>
void HillclimbAssertions< Krah1941DictsPolicy >::RunFinalAssertions( const Key& gkey ) const {
    const ExpectedKey expected = {
        .score = 17930,
        .ukw = {.type = UkwType::UkwType_B},
        .slot = {{GreekRingType::GreekRingType_None},
                 {RingType::RingType_5},
                 {RingType::RingType_3},
                 {RingType::RingType_2}},
        .stecker = "AIBECJDRFYGOHZMUNQPVST",
        .ring = "AAC",
        .mesg = "HVS"};

    expected.AssertMatch( gkey );
}
