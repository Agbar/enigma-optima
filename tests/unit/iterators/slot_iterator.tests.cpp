#include <gtest/gtest.h>


extern "C" {
#include "echar/echar_delta.h"
#include "iterators/slot_iterator.h"
}


static RingTypes operator"" _ringTypes_H( const char* txt, std::size_t len ) {
    auto alpha_to_RingType = []( const char c ) -> RingType {
        if( '0' > c || c > '5' ) throw std::logic_error( "Invalid rings literal." );
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


static inline bool operator==( const RingTypes lhs, const RingTypes rhs ) {
    return RingTypes_equ( lhs, rhs );
}


class slot_iterator_h
    : public ::testing::Test {
protected:
    RingTypes slot{
        g : {GreekRingType_None},
        l : {RingType_1},
        m : {RingType_2},
        r : {RingType_3}
    };
    const ModelType_t model = EnigmaModel_H;
};


TEST_F( slot_iterator_h, init_111_selects_123 ) {
    slot = "111"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    ASSERT_EQ( *slot_iter.state, "123"_ringTypes_H );
}


TEST_F( slot_iterator_h, count_all ) {
    uint64_t loop_count = 0;
    for( SlotIterator slot_iter = init_SlotIterator( &slot, model );
         !SlotIterator_equ( slot_overflow(), slot_iter );
         slot_iter.next( &slot_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 5 * 4 * 3 );
}


TEST_F( slot_iterator_h, next_after_123_is_124 ) {
    slot = "123"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "124"_ringTypes_H );
}


TEST_F( slot_iterator_h, next_after_125_is_132 ) {
    slot = "125"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "132"_ringTypes_H );
}


TEST_F( slot_iterator_h, next_after_132_is_134 ) {
    slot = "132"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "134"_ringTypes_H );
}


TEST_F( slot_iterator_h, next_after_215_is_231 ) {
    slot = "215"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "231"_ringTypes_H );
}


TEST_F( slot_iterator_h, next_after_231_is_234 ) {
    slot = "231"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "234"_ringTypes_H );
}


TEST_F( slot_iterator_h, next_after_543_is_123_with_overflow ) {
    slot = "543"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "123"_ringTypes_H );
    ASSERT_TRUE( slot_iter.bits.og & SlotIterator_bits_overflow );
    ASSERT_TRUE( SlotIterator_equ( slot_iter, slot_overflow() ) );
}
