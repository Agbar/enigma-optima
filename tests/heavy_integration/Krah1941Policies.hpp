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
    SCOPED_TRACE( "OnNewBest[" + std::to_string( new_best_cnt++ ) + "]" );
    ASSERT_LE( new_best_cnt, best_score_keys.size() );
    AssertMatch( gkey, best_score_keys.at( new_best_cnt - 1 ), expected_rings );
}


template<>
const std::vector< heavy_test::ExpectedKeyState >
    HillclimbAssertions< Krah1941DictsPolicy >::
        best_score_keys{
            {.score = 8995, .stecker = "BECODRGPHKILJXMVQSYZ", .ring = "AAA", .mesg = "AVA"},
            {.score = 11010, .stecker = "ANBMDOFUHPJYKSLZQVRW", .ring = "AAA", .mesg = "AVB"},
            {.score = 11557, .stecker = "AHBSCYDZERFPGVKTLUMONQ", .ring = "AAA", .mesg = "AVF"},
            {.score = 12130, .stecker = "ASBLDQEHFPJKMWOTRZVY", .ring = "AAA", .mesg = "AVH"},
            {.score = 13115, .stecker = "ATBHDJFSGWLVMQOXPR", .ring = "AAA", .mesg = "AVK"},
            {.score = 13480, .stecker = "APBECDFKIJLZRSXY", .ring = "AAA", .mesg = "AVU"},
            {.score = 14650, .stecker = "AHBQCSDFELGWKROTPVXY", .ring = "AAA", .mesg = "AVX"},
            {.score = 14755, .stecker = "AWBFCNDPEHGIJRKOLVMYTZUX", .ring = "AAA", .mesg = "CVU"},
            {.score = 15560, .stecker = "ADBNCWESITJMKQLOPVRZUY", .ring = "AAA", .mesg = "EVZ"},
            {.score = 16655, .stecker = "ANBRCODGEUFJKPMZQWSV", .ring = "AAA", .mesg = "XVR"},
            {.score = 17019, .stecker = "AWCEGTHJKVLNMUOR", .ring = "AAB", .mesg = "CVV"},
            {.score = 17930, .stecker = "AIBECJDRFYGOHZMUNQPVST", .ring = "AAC", .mesg = "HVS"}};
