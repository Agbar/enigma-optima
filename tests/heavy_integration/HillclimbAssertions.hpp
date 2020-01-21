#pragma once

#include "ExpectedKey.hpp"

extern "C" {
#include "global.h"
#include "key.h"
}


template< class DictOptPlicy >
struct HillclimbAssertions {
    void RunFinalAssertions( const Key& gkey ) const;
    void OnNewBest( const Key& gkey );

private:
    static const heavy_test::ExpectedRings expected_rings;
    static const std::vector< heavy_test::ExpectedKeyState > best_score_keys;
    unsigned int new_best_cnt = 0;
};


template< class D >
void HillclimbAssertions< D >::OnNewBest( UNUSED const Key& gkey ) {
    new_best_cnt++;
}
