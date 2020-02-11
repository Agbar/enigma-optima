#include <gtest/gtest.h>


extern "C" {
#include "echar/echar_delta.h"
#include "iterators/mesg_iterator.h"
}


TEST( mesg_iterator, count_all_m3 ) {
    RingsState mesg{
        g : make_echar_delta_ascii( 'A' ),
        l : make_echar_delta_ascii( 'A' ),
        m : make_echar_delta_ascii( 'A' ),
        r : make_echar_delta_ascii( 'A' ),
    };
    struct MesgIterator mesg_iter = {state : &mesg, overflow : false};
    const struct MesgIterator mesg_end = {state : nullptr, overflow : true};
    uint64_t loop_count = 0;
    for( ; !MesgIterator_equ( mesg_end, mesg_iter );
         mesg_iter = next_mesg( mesg_iter, EnigmaModel_M3 ) ) {
        loop_count++;
    }
    ASSERT_EQ( loop_count, 26 * 26 * 26 );
}


TEST( mesg_iterator, count_all_m4 ) {
    RingsState mesg{
        g : make_echar_delta_ascii( 'A' ),
        l : make_echar_delta_ascii( 'A' ),
        m : make_echar_delta_ascii( 'A' ),
        r : make_echar_delta_ascii( 'A' ),
    };
    struct MesgIterator mesg_iter = {state : &mesg, overflow : false};
    const struct MesgIterator mesg_end = {state : nullptr, overflow : true};
    uint64_t loop_count = 0;
    for( ; !MesgIterator_equ( mesg_end, mesg_iter );
         mesg_iter = next_mesg( mesg_iter, EnigmaModel_M4 ) ) {
        loop_count++;
    }
    ASSERT_EQ( loop_count, 26 * 26 * 26 * 26 );
}
