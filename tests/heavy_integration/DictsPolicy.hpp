#pragma once


#include "cstring"
#include "gtest/gtest.h"

namespace heavy_test {

bool load_tridict( const char text[] );
bool load_bidict( const char text[] );
void clear_tridict();
void clear_bidict();

} // namespace heavy_test


template< const char ( &trigraph_txt )[], const char ( &bigraph_txt )[] >
struct KrahDictsPolicy {
    static void LoadDicts() {
        ASSERT_TRUE( heavy_test::load_tridict( trigraph_txt ) );
        ASSERT_TRUE( heavy_test::load_bidict( bigraph_txt ) );
    }

    static void ClearDicts() {
        heavy_test::clear_tridict();
        heavy_test::clear_bidict();
    }
};
