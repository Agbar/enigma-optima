#pragma once


#include "cstring"
#include "gtest/gtest.h"

extern "C" {
#include "stbrett/krah_optimizer.h"
#include "stbrett/optimizer.h"
#include "stbrett/ve3nea_optimizer.h"
}


namespace heavy_test {

bool load_tridict( const char text[] );
bool load_bidict( const char text[] );
bool load_unidict( const char text[] );
void clear_tridict();
void clear_bidict();
void clear_unidict();


} // namespace heavy_test


template< const char ( &trigraph_txt )[], const char ( &bigraph_txt )[] >
struct KrahDictsPolicy {
    static constexpr stbrett_optimize_f& stbrett_optimizer = stbrett_optimize_krah;

    static void LoadDicts() {
        ASSERT_TRUE( heavy_test::load_tridict( trigraph_txt ) );
        ASSERT_TRUE( heavy_test::load_bidict( bigraph_txt ) );
    }

    static void ClearDicts() {
        heavy_test::clear_tridict();
        heavy_test::clear_bidict();
    }
};


template< const char ( &trigraph_txt )[], const char ( &unigraph_txt )[] >
struct Ve3neaDictsPolicy {
    static constexpr stbrett_optimize_f& stbrett_optimizer = stbrett_optimize_ve3nea;

    static void LoadDicts() {
        ASSERT_TRUE( heavy_test::load_tridict( trigraph_txt ) );
        ASSERT_TRUE( heavy_test::load_unidict( unigraph_txt ) );
    }

    static void ClearDicts() {
        heavy_test::clear_tridict();
        heavy_test::clear_unidict();
    }
};
