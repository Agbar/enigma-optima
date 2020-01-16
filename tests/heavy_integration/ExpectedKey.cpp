#include "ExpectedKey.hpp"

#include <gtest/gtest.h>


void ExpectedKey::AssertMatch( const Key& k ) const {
    EXPECT_EQ( k.score, score );

    EXPECT_EQ( k.ukwnum.type, ukw.type );
    auto& kslot = k.slot;
    EXPECT_EQ( kslot.l.type, slot.l.type );
    EXPECT_EQ( kslot.m.type, slot.m.type );
    EXPECT_EQ( kslot.r.type, slot.r.type );

    std::string actual_stecker;
    std::transform(
        std::begin( k.sf.map ),
        std::begin( k.sf.map ) + k.count,
        std::back_inserter( actual_stecker ),
        []( const echar& e ) { return echar_to_ALPHA( e ); }
    );
    EXPECT_EQ( actual_stecker, stecker );

    char actual_ring[]{
        echar_delta_to_ALPHA( k.ring.l ),
        echar_delta_to_ALPHA( k.ring.m ),
        echar_delta_to_ALPHA( k.ring.r ), '\0'};
    EXPECT_EQ( actual_ring, ring );
    char actual_mesg[]{
        echar_delta_to_ALPHA( k.mesg.l ),
        echar_delta_to_ALPHA( k.mesg.m ),
        echar_delta_to_ALPHA( k.mesg.r ), '\0'};
    EXPECT_EQ( actual_mesg, mesg );
}
