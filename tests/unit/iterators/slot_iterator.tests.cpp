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


static RingTypes operator"" _ringTypes_M3( const char* txt, std::size_t len ) {
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


static RingTypes operator"" _ringTypes_M4( const char* txt, std::size_t len ) {
    auto alpha_to_RingType = []( const char c ) -> RingType {
        if( '0' > c || c > '8' ) throw std::logic_error( "Invalid rings literal." );
        return RingType{static_cast< ring_type_enum >( c - '0' )};
    };
    auto alpha_to_GreekRingType = []( const char c ) -> GreekRingType {
        switch( c ) {
        case 'B':
            return GreekRingType{GreekRingType_Beta};
        case 'G':
            return GreekRingType{GreekRingType_Gamma};
        default:
            throw std::logic_error( "Invalid rings literal." );
        }
    };

    if( len == 4 ) {
        return RingTypes{.g = alpha_to_GreekRingType( txt[ 0 ] ),
                         .l = alpha_to_RingType( txt[ 1 ] ),
                         .m = alpha_to_RingType( txt[ 2 ] ),
                         .r = alpha_to_RingType( txt[ 3 ] )};
    } else {
        throw std::logic_error( "Invalid rings literal. Length=" + std::to_string( len ) );
    }
}


static inline bool operator==( const RingTypes lhs, const RingTypes rhs ) {
    return RingTypes_equ( lhs, rhs );
}


TEST( SlotIterator_equ_H, 132_equ_132 ) {
    const ModelType_t model = EnigmaModel_H;
    RingTypes slot = "125"_ringTypes_H;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );
    slot_iter.next( &slot_iter );

    RingTypes expected_132 = "132"_ringTypes_H;
    EXPECT_EQ( *slot_iter.state, expected_132 );
    ASSERT_TRUE( SlotIterator_equ( slot_iter, init_SlotIterator( &expected_132, model ) ) );
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

//============================


TEST( SlotIterator_equ_M3, 132_equ_132 ) {
    const ModelType_t model = EnigmaModel_M3;
    RingTypes slot = "128"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );
    slot_iter.next( &slot_iter );

    RingTypes expected_132 = "132"_ringTypes_M3;
    EXPECT_EQ( *slot_iter.state, expected_132 );
    ASSERT_TRUE( SlotIterator_equ( slot_iter, init_SlotIterator( &expected_132, model ) ) );
}


class slot_iterator_m3
    : public ::testing::Test {
protected:
    RingTypes slot{
        g : {GreekRingType_None},
        l : {RingType_1},
        m : {RingType_2},
        r : {RingType_3}
    };
    const ModelType_t model = EnigmaModel_M3;
};


TEST_F( slot_iterator_m3, init_111_selects_123 ) {
    slot = "111"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    ASSERT_EQ( *slot_iter.state, "123"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, count_all ) {
    uint64_t loop_count = 0;
    for( SlotIterator slot_iter = init_SlotIterator( &slot, model );
         !SlotIterator_equ( slot_overflow(), slot_iter );
         slot_iter.next( &slot_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 8 * 7 * 6 );
}


TEST_F( slot_iterator_m3, next_after_123_is_124 ) {
    slot = "123"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "124"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, next_after_125_is_126 ) {
    slot = "125"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "126"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, next_after_128_is_132 ) {
    slot = "128"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "132"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, next_after_132_is_134 ) {
    slot = "132"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "134"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, next_after_218_is_231 ) {
    slot = "218"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "231"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, next_after_675_is_678 ) {
    slot = "675"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "678"_ringTypes_M3 );
}


TEST_F( slot_iterator_m3, next_after_876_is_123_with_overflow ) {
    slot = "876"_ringTypes_M3;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "123"_ringTypes_M3 );
    ASSERT_TRUE( slot_iter.bits.og & SlotIterator_bits_overflow );
    ASSERT_TRUE( SlotIterator_equ( slot_iter, slot_overflow() ) );
}


//============================


TEST( SlotIterator_equ_M4, 132_equ_132 ) {
    const ModelType_t model = EnigmaModel_M4;
    RingTypes slot = "B128"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );
    slot_iter.next( &slot_iter );

    RingTypes expected_132 = "B132"_ringTypes_M4;
    EXPECT_EQ( *slot_iter.state, expected_132 );
    ASSERT_TRUE( SlotIterator_equ( slot_iter, init_SlotIterator( &expected_132, model ) ) );
}


class slot_iterator_m4
    : public ::testing::Test {
protected:
    RingTypes slot{
        g : {GreekRingType_Beta},
        l : {RingType_1},
        m : {RingType_2},
        r : {RingType_3}
    };
    const ModelType_t model = EnigmaModel_M4;
};


TEST_F( slot_iterator_m4, init_B111_selects_B123 ) {
    slot = "B111"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    ASSERT_EQ( *slot_iter.state, "B123"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, count_all ) {
    uint64_t loop_count = 0;
    for( SlotIterator slot_iter = init_SlotIterator( &slot, model );
         !SlotIterator_equ( slot_overflow(), slot_iter );
         slot_iter.next( &slot_iter ) ) {
        loop_count++;
    }

    ASSERT_EQ( loop_count, 2 * 8 * 7 * 6 );
}


TEST_F( slot_iterator_m4, next_after_B123_is_B124 ) {
    slot = "B123"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B124"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, next_after_B125_is_B126 ) {
    slot = "B125"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B126"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, next_after_B128_is_B132 ) {
    slot = "B128"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B132"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, next_after_B132_is_B134 ) {
    slot = "B132"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B134"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, next_after_B218_is_B231 ) {
    slot = "B218"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B231"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, next_after_B675_is_B678 ) {
    slot = "B675"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B678"_ringTypes_M4 );
}


TEST_F( slot_iterator_m4, next_after_B876_is_G123 ) {
    slot = "B876"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "G123"_ringTypes_M4 );
    ASSERT_FALSE( slot_iter.bits.og & SlotIterator_bits_overflow );
}


TEST_F( slot_iterator_m4, next_after_G876_is_B123_with_overflow ) {
    slot = "G876"_ringTypes_M4;
    SlotIterator slot_iter = init_SlotIterator( &slot, model );

    slot_iter.next( &slot_iter );

    ASSERT_EQ( *slot_iter.state, "B123"_ringTypes_M4 );
    ASSERT_TRUE( slot_iter.bits.og & SlotIterator_bits_overflow );
    ASSERT_TRUE( SlotIterator_equ( slot_iter, slot_overflow() ) );
}
