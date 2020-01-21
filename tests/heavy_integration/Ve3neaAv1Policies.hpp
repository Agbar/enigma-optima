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


template<>
void HillclimbAssertions< Ve3neaAv1DictsPolicy >::OnNewBest( UNUSED const Key& gkey ) {
    SCOPED_TRACE( "OnNewBest[" + std::to_string( new_best_cnt++ ) + "]" );
    ASSERT_LE( new_best_cnt, best_score_keys.size() );
    AssertMatch( gkey, best_score_keys.at( new_best_cnt - 1 ), expected_rings );
}


template<>
const std::vector< heavy_test::ExpectedKeyState >
    HillclimbAssertions< Ve3neaAv1DictsPolicy >::
        best_score_keys{
            {.score = 46438, .stecker = "AXBZCNDPEOJMLSQV", .ring = "AAA", .mesg = "AVA"},
            {.score = 51950, .stecker = "AXBECIDHFLJWKVNTOSPYQRUZ", .ring = "AAA", .mesg = "AVB"},
            {.score = 52346, .stecker = "CTDXHWISNOPZRUVY", .ring = "AAA", .mesg = "BVH"},
            {.score = 52403, .stecker = "AVBGCKHPIUMNOTQYXZ", .ring = "AAA", .mesg = "BVK"},
            {.score = 52406, .stecker = "ADBVCKEGIYJULRNXOSQW", .ring = "AAA", .mesg = "EVV"},
            {.score = 52591, .stecker = "AUCRDWEOFKGJNSPTXY", .ring = "AAA", .mesg = "GVL"},
            {.score = 52851, .stecker = "BZCXDFGUHOISMTNYQRVW", .ring = "AAA", .mesg = "HVA"},
            {.score = 53198, .stecker = "BNCWEUFTGIJMKQPYSV", .ring = "AAA", .mesg = "NVD"},
            {.score = 53258, .stecker = "ARCTDLHQIPJOSXYZ", .ring = "AAA", .mesg = "SVJ"},
            {.score = 56793, .stecker = "AUDLESFWGOKPMNTZ", .ring = "AAA", .mesg = "UVC"},
            {.score = 56817, .stecker = "ACDREXFGIPJWLVNQSZ", .ring = "AAB", .mesg = "FVM"},
            {.score = 56961, .stecker = "CUEZFSGQHPIWKOMR", .ring = "AAB", .mesg = "GVQ"},
            {.score = 57037, .stecker = "AUBGCNDEFKHVIRJZMSOTPQWY", .ring = "AAB", .mesg = "UVE"},
            {.score = 58844, .stecker = "AHBTCEFUISKXMONVPQRW", .ring = "AAD", .mesg = "RVM"},
            {.score = 59791, .stecker = "AGBYCZEJFVKNLPOT", .ring = "AAE", .mesg = "VVK"}};
