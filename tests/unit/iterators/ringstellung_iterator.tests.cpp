#include <gtest/gtest.h>


extern "C" {
#include "echar/echar_delta.h"
#include "iterators/ringstellung_iterator.h"
}


TEST( ringstellung_iterator, count_all_rings_I_and_V ) {
    Ringstellung ringstellung{
        m : make_echar_delta_ascii( 'A' ),
        r : make_echar_delta_ascii( 'A' ),
    };

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_1},
        r : RingType{RingType_5},
        overflow : false,
    };
    const struct RingstellungIterator ring_end = {
        state : nullptr,
        m : {RingType_None},
        r : {RingType_None},
        overflow : true,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_end, ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 26 * 26 );
}


TEST( ringstellung_iterator, count_all_rings_I_and_VI ) {
    Ringstellung ringstellung{
        m : make_echar_delta_ascii( 'A' ),
        r : make_echar_delta_ascii( 'A' ),
    };

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_1},
        r : RingType{RingType_6},
        overflow : false,
    };
    const struct RingstellungIterator ring_end = {
        state : nullptr,
        m : {RingType_None},
        r : {RingType_None},
        overflow : true,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_end, ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 26 * 13 );
}


TEST( ringstellung_iterator, count_all_rings_VI_and_V ) {
    Ringstellung ringstellung{
        m : make_echar_delta_ascii( 'A' ),
        r : make_echar_delta_ascii( 'A' ),
    };

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_6},
        r : RingType{RingType_5},
        overflow : false,
    };
    const struct RingstellungIterator ring_end = {
        state : nullptr,
        m : {RingType_None},
        r : {RingType_None},
        overflow : true,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_end, ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 13 * 26 );
}


TEST( ringstellung_iterator, count_all_rings_VI_and_VIII ) {
    Ringstellung ringstellung{
        m : make_echar_delta_ascii( 'A' ),
        r : make_echar_delta_ascii( 'A' ),
    };

    struct RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_6},
        r : RingType{RingType_8},
        overflow : false,
    };
    const struct RingstellungIterator ring_end = {
        state : nullptr,
        m : {RingType_None},
        r : {RingType_None},
        overflow : true,
    };
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_end, ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 13 * 13 );
}
