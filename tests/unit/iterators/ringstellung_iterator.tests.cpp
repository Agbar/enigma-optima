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


static inline bool operator==( const Ringstellung lhs, const Ringstellung rhs ) {
    return Ringstellung_equ( lhs, rhs );
}


class ringstellung_iterator_I_and_V
    : public ::testing::Test {
protected:
    Ringstellung ringstellung = "AA"_ringstellung;
    RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_1},
        r : RingType{RingType_5},
        overflow : false,
    };
};


TEST_F( ringstellung_iterator_I_and_V, count_all ) {
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;

        // defend against infinite loops
        if( loop_count > 2 * ( 26 * 26 ) ) break;
    }

    ASSERT_EQ( loop_count, 26 * 26 );
}


TEST_F( ringstellung_iterator_I_and_V, next_after_AA_is_AB ) {
    ringstellung = "AA"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AB"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_V, next_after_AM_is_AN ) {
    ringstellung = "AM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AN"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_V, next_after_AZ_is_BA ) {
    ringstellung = "AZ"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "BA"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_V, next_after_MZ_is_NA ) {
    ringstellung = "MZ"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "NA"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_V, next_after_ZZ_is_AA_with_overflow ) {
    ringstellung = "ZZ"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AA"_ringstellung );
    ASSERT_TRUE( ring_iter.overflow );
}


class ringstellung_iterator_I_and_VI
    : public ::testing::Test {
protected:
    Ringstellung ringstellung = "AA"_ringstellung;
    RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_1},
        r : RingType{RingType_6},
        overflow : false,
    };
};


TEST_F( ringstellung_iterator_I_and_VI, count_all ) {
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;

        // defend against infinite loops
        if( loop_count > 2 * ( 26 * 13 ) ) break;
    }

    ASSERT_EQ( loop_count, 26 * 13 );
}


TEST_F( ringstellung_iterator_I_and_VI, next_after_AA_is_AB ) {
    ringstellung = "AA"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AB"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_VI, next_after_AM_is_BA ) {
    ringstellung = "AM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "BA"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_VI, next_after_MM_is_NA ) {
    ringstellung = "MM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "NA"_ringstellung );
}


TEST_F( ringstellung_iterator_I_and_VI, next_after_ZM_is_AA_with_overflow ) {
    ringstellung = "ZM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AA"_ringstellung );
    ASSERT_TRUE( ring_iter.overflow );
}


class ringstellung_iterator_VI_and_V
    : public ::testing::Test {
protected:
    Ringstellung ringstellung = "AA"_ringstellung;
    RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_6},
        r : RingType{RingType_5},
        overflow : false,
    };
};


TEST_F( ringstellung_iterator_VI_and_V, count_all ) {
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;

        // defend against infinite loops
        if( loop_count > 2 * ( 13 * 26 ) ) break;
    }

    ASSERT_EQ( loop_count, 13 * 26 );
}


TEST_F( ringstellung_iterator_VI_and_V, next_after_AA_is_AB ) {
    ringstellung = "AA"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AB"_ringstellung );
}


TEST_F( ringstellung_iterator_VI_and_V, next_after_AM_is_AN ) {
    ringstellung = "AM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AN"_ringstellung );
}


TEST_F( ringstellung_iterator_VI_and_V, next_after_AZ_is_BA ) {
    ringstellung = "AZ"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "BA"_ringstellung );
}


TEST_F( ringstellung_iterator_VI_and_V, next_after_MZ_is_AA_with_overflow ) {
    ringstellung = "MZ"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AA"_ringstellung );
    ASSERT_TRUE( ring_iter.overflow );
}


class ringstellung_iterator_VI_and_VIII
    : public ::testing::Test {
protected:
    Ringstellung ringstellung = "AA"_ringstellung;
    RingstellungIterator ring_iter = {
        state : &ringstellung,
        m : RingType{RingType_6},
        r : RingType{RingType_8},
        overflow : false,
    };
};


TEST_F( ringstellung_iterator_VI_and_VIII, count_all ) {
    uint64_t loop_count = 0;
    for( ; !RingstellungIterator_equ( ring_overflow(), ring_iter );
         next_ringstellung( &ring_iter ) ) {
        loop_count++;

        // defend against infinite loops
        if( loop_count > 2 * ( 13 * 13 ) ) break;
    }

    ASSERT_EQ( loop_count, 13 * 13 );
}


TEST_F( ringstellung_iterator_VI_and_VIII, next_after_AA_is_AB ) {
    ringstellung = "AA"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AB"_ringstellung );
}


TEST_F( ringstellung_iterator_VI_and_VIII, next_after_AM_is_BA ) {
    ringstellung = "AM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "BA"_ringstellung );
}


TEST_F( ringstellung_iterator_VI_and_VIII, next_after_MA_is_MB ) {
    ringstellung = "MA"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "MB"_ringstellung );
}


TEST_F( ringstellung_iterator_VI_and_VIII, next_after_MM_is_AA_with_overflow ) {
    ringstellung = "MM"_ringstellung;
    next_ringstellung( &ring_iter );

    ASSERT_EQ( *ring_iter.state, "AA"_ringstellung );
    ASSERT_TRUE( ring_iter.overflow );
}
