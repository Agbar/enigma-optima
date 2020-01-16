#include "ExpectedKey.hpp"

#include <gtest/gtest.h>
#include <string>

namespace heavy_test {

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

RingTypes operator"" _ringTypes( const char* txt, std::size_t len ) {
    auto alpha_to_RingType = []( const char c ) -> RingType {
        if( '0' > c || c > '8' ) throw std::logic_error( "Invalid rings literal." );
        return RingType{static_cast< RingType::ring_type_enum >( c - '0' )};
    };

    if( len == 3 ) {
        return RingTypes{.g = {GreekRingType::GreekRingType_None},
                         .l = alpha_to_RingType( txt[ 0 ] ),
                         .m = alpha_to_RingType( txt[ 1 ] ),
                         .r = alpha_to_RingType( txt[ 2 ] )};
    } else {
        throw std::logic_error( "Invalid rings literal. Length=" + std::to_string( len ) );
    }
}


} // namespace heavy_test