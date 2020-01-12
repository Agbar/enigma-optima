#pragma once


#include "DictsPolicy.hpp"
#include "HillclimbAssertions.hpp"


extern "C" {
#include "enigma/test_data.h"
}


using Krah1941DictsPolicy = KrahDictsPolicy< trigraph_1941, bigraph_1941 >;


template<>
void Assertions< Krah1941DictsPolicy >::RunFinalAssertions( const Key& gkey ) {
    EXPECT_EQ( gkey.score, 17930 );

    EXPECT_EQ( gkey.ukwnum.type, UkwType::UkwType_B );
    auto& slot = gkey.slot;
    EXPECT_EQ( slot.l.type, RingType::RingType_5 );
    EXPECT_EQ( slot.m.type, RingType::RingType_3 );
    EXPECT_EQ( slot.r.type, RingType::RingType_2 );

    std::string stecker;
    std::transform(
        std::begin( gkey.sf.map ),
        std::begin( gkey.sf.map ) + gkey.count,
        std::back_inserter( stecker ),
        []( const echar& e ) { return echar_to_ALPHA( e ); }
    );
    EXPECT_EQ( stecker, "AIBECJDRFYGOHZMUNQPVST" );

    char ring[]{
        echar_delta_to_ALPHA( gkey.ring.l ),
        echar_delta_to_ALPHA( gkey.ring.m ),
        echar_delta_to_ALPHA( gkey.ring.r ), '\0'};
    EXPECT_STREQ( ring, "AAC" );
    char mesg[]{
        echar_delta_to_ALPHA( gkey.mesg.l ),
        echar_delta_to_ALPHA( gkey.mesg.m ),
        echar_delta_to_ALPHA( gkey.mesg.r ), '\0'};
    EXPECT_STREQ( mesg, "HVS" );
}
