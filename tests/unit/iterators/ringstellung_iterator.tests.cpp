#include <gtest/gtest.h>


extern "C" {
#include "echar/echar_delta.h"
#include "iterators/ringstellung_iterator.h"
}


static Ringstellung operator"" _ringstellung( const char txt[ 4 ], std::size_t len ) {
    auto alpha_to_echar_delta = []( const char c ) -> echar_delta {
        if( !echar_delta_can_make_from_ascii( c ) ) throw std::logic_error( "Invalid rings literal." );
        return make_echar_delta_ascii( c );
    };

    if( len == 2 ) {
        return Ringstellung{
            .m = alpha_to_echar_delta( txt[ 0 ] ),
            .r = alpha_to_echar_delta( txt[ 1 ] )};
    } else {
        throw std::logic_error( "Invalid ringstellung literal. Length=" + std::to_string( len ) );
    }
}


TEST( ringstellung_iterator, count_all_rings_I_and_V ) {
    Ringstellung ringstellung = "AA"_ringstellung;

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_1},
        r : RingType{RingType_5},
        overflow : false,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 26 * 26 );
}


    Ringstellung ringstellung = "AA"_ringstellung;

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_1},
        r : RingType{RingType_6},
        overflow : false,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 26 * 13 );
}


    Ringstellung ringstellung = "AA"_ringstellung;

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_6},
        r : RingType{RingType_5},
        overflow : false,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 13 * 26 );
}


    Ringstellung ringstellung = "AA"_ringstellung;

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_6},
        r : RingType{RingType_8},
        overflow : false,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 13 * 13 );
}
