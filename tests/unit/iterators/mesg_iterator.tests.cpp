#include <gtest/gtest.h>


extern "C" {
#include "echar/echar_delta.h"
#include "iterators/mesg_iterator.h"
}

bool operator==( const RingsState lhs, const RingsState rhs );


static RingsState operator"" _mesg_m3( const char txt[ 4 ], std::size_t len ) {
    auto alpha_to_echar_delta = []( const char c ) -> echar_delta {
        if( !echar_delta_can_make_from_ascii( c ) ) throw std::logic_error( "Invalid rings literal." );
        return make_echar_delta_ascii( c );
    };

    if( len == 3 ) {
        return RingsState{.g = {0},
                          .l = alpha_to_echar_delta( txt[ 0 ] ),
                          .m = alpha_to_echar_delta( txt[ 1 ] ),
                          .r = alpha_to_echar_delta( txt[ 2 ] )};
    } else {
        throw std::logic_error( "Invalid rings literal. Length=" + std::to_string( len ) );
    }
}


static RingsState operator"" _mesg_m4( const char txt[ 5 ], std::size_t len ) {
    auto alpha_to_echar_delta = []( const char c ) -> echar_delta {
        if( !echar_delta_can_make_from_ascii( c ) ) throw std::logic_error( "Invalid rings literal." );
        return make_echar_delta_ascii( c );
    };

    if( len == 4 ) {
        return RingsState{.g = alpha_to_echar_delta( txt[ 0 ] ),
                          .l = alpha_to_echar_delta( txt[ 1 ] ),
                          .m = alpha_to_echar_delta( txt[ 2 ] ),
                          .r = alpha_to_echar_delta( txt[ 3 ] )};
    } else {
        throw std::logic_error( "Invalid rings literal. Length=" + std::to_string( len ) );
    }
}


bool operator==( const RingsState lhs, const RingsState rhs ) {
    return lhs.g.delta == rhs.g.delta
           && lhs.l.delta == rhs.l.delta
           && lhs.m.delta == rhs.m.delta
           && lhs.r.delta == rhs.r.delta;
}


TEST( mesg_iterator_m3, count_all ) {
    RingsState mesg = "AAA"_mesg_m3;
    struct MesgIterator mesg_iter = {state : &mesg, overflow : false};
    uint64_t loop_count = 0;
    for( ; !MesgIterator_equ( mesg_overflow(), mesg_iter );
         mesg_iter = next_mesg( mesg_iter, EnigmaModel_M3 ) ) {
        loop_count++;

        // defend against infinite loops
        if( loop_count > 2 * ( 26 * 26 * 26 ) ) break;
    }
    EXPECT_EQ( *mesg_iter.state, "AAA"_mesg_m3 );
    ASSERT_EQ( loop_count, 26 * 26 * 26 );
}


TEST( mesg_iterator_m3, next_mesg_after_AAA_is_AAB ) {
    RingsState mesg = "AAA"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    const RingsState expected = "AAB"_mesg_m3;
    ASSERT_EQ( *result.state, expected );
    EXPECT_FALSE( result.overflow );
}


TEST( mesg_iterator_m3, next_mesg_after_AAY_is_AAZ ) {
    RingsState mesg = "AAY"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    const RingsState expected = "AAZ"_mesg_m3;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m3, next_mesg_after_AAZ_is_ABA ) {
    RingsState mesg = "AAZ"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    const RingsState expected = "ABA"_mesg_m3;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m3, next_mesg_after_ABA_is_ABB ) {
    RingsState mesg = "ABA"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    const RingsState expected = "ABB"_mesg_m3;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m3, next_mesg_after_AZZ_is_BAA ) {
    RingsState mesg = "AZZ"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    const RingsState expected = "BAA"_mesg_m3;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m3, next_mesg_after_ZAZ_is_ZBA ) {
    RingsState mesg = "ZAZ"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    const RingsState expected = "ZBA"_mesg_m3;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m3, next_mesg_after_ZZZ_is_AAA_with_overflow ) {
    RingsState mesg = "ZZZ"_mesg_m3;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M3 );

    ASSERT_TRUE( result.overflow );
    ASSERT_TRUE( MesgIterator_equ( result, mesg_overflow() ) );
    const RingsState aaa = "AAA"_mesg_m3;
    ASSERT_EQ( *result.state, aaa );
}


TEST( mesg_iterator_m4, count_all_m4 ) {
    RingsState mesg = "AAAA"_mesg_m4;
    struct MesgIterator mesg_iter = {state : &mesg, overflow : false};
    uint64_t loop_count = 0;
    for( ; !MesgIterator_equ( mesg_overflow(), mesg_iter );
         mesg_iter = next_mesg( mesg_iter, EnigmaModel_M4 ) ) {
        loop_count++;

        // defend against infinite loops
        if( loop_count > 2 * ( 26 * 26 * 26 * 26 ) ) break;
    }
    EXPECT_EQ( *mesg_iter.state, "AAAA"_mesg_m4 );
    ASSERT_EQ( loop_count, 26 * 26 * 26 * 26 );
}


TEST( mesg_iterator_m4, next_mesg_after_AAAA_is_AAAB ) {
    RingsState mesg = "AAAA"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "AAAB"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
    EXPECT_FALSE( result.overflow );
}


TEST( mesg_iterator_m4, next_mesg_after_AAAZ_is_AABA ) {
    RingsState mesg = "AAAZ"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "AABA"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_AABA_is_AABB ) {
    RingsState mesg = "AABA"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "AABB"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_AAZZ_is_ABAA ) {
    RingsState mesg = "AAZZ"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "ABAA"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_ABAA_is_ABAB ) {
    RingsState mesg = "ABAA"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "ABAB"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_AZAZ_is_AZBA ) {
    RingsState mesg = "AZAZ"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "AZBA"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_AZZZ_is_BAAA ) {
    RingsState mesg = "AZZZ"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "BAAA"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_ZAAZ_is_ZABA ) {
    RingsState mesg = "ZAAZ"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "ZABA"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
}


TEST( mesg_iterator_m4, next_mesg_after_ZZZZ_is_AAAA_with_overflow ) {
    RingsState mesg = "ZZZZ"_mesg_m4;
    const MesgIterator mesg_iter{state : &mesg, overflow : false};

    MesgIterator result = next_mesg( mesg_iter, EnigmaModel_M4 );

    const RingsState expected = "AAAA"_mesg_m4;
    ASSERT_EQ( *result.state, expected );
    ASSERT_TRUE( result.overflow );
    ASSERT_TRUE( MesgIterator_equ( result, mesg_overflow() ) );
}
