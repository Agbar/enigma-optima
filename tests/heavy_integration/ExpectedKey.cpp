#include "ExpectedKey.hpp"

#include <gtest/gtest.h>
#include <string>

namespace heavy_test {

void AssertMatch( const Key& actual_key,
                  const ExpectedKeyState& x_state,
                  const ExpectedRings& x_rings ) {
    EXPECT_EQ( actual_key.score, x_state.score );

    EXPECT_EQ( actual_key.ukwnum.type, x_rings.ukw.type );
    EXPECT_EQ( actual_key.slot.l.type, x_rings.slot.l.type );
    EXPECT_EQ( actual_key.slot.m.type, x_rings.slot.m.type );
    EXPECT_EQ( actual_key.slot.r.type, x_rings.slot.r.type );

    std::string actual_stecker;
    std::transform(
        std::begin( actual_key.sf.map ),
        std::begin( actual_key.sf.map ) + actual_key.count,
        std::back_inserter( actual_stecker ),
        []( const echar& e ) { return echar_to_ALPHA( e ); } );
    EXPECT_EQ( actual_stecker, x_state.stecker );

    char actual_ring[]{
        'A',
        echar_delta_to_ALPHA( actual_key.ring.m ),
        echar_delta_to_ALPHA( actual_key.ring.r ), '\0'};
    EXPECT_EQ( actual_ring, x_state.ring );
    char actual_mesg[]{
        echar_delta_to_ALPHA( actual_key.mesg.l ),
        echar_delta_to_ALPHA( actual_key.mesg.m ),
        echar_delta_to_ALPHA( actual_key.mesg.r ), '\0'};
    EXPECT_EQ( actual_mesg, x_state.mesg );
}

RingTypes operator"" _ringTypes( const char* txt, std::size_t len ) {
    auto alpha_to_RingType = []( const char c ) -> RingType {
        if( '0' > c || c > '8' ) throw std::logic_error( "Invalid rings literal." );
        return RingType{static_cast< ring_type_enum >( c - '0' )};
    };

    if( len == 3 ) {
        return RingTypes{.g = {GreekRingType_None},
                         .l = alpha_to_RingType( txt[ 0 ] ),
                         .m = alpha_to_RingType( txt[ 1 ] ),
                         .r = alpha_to_RingType( txt[ 2 ] )};
    } else {
        throw std::logic_error( "Invalid rings literal. Length=" + std::to_string( len ) );
    }
}


} // namespace heavy_test