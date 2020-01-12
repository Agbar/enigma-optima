#pragma once


extern "C" {
#include "global.h"
#include "key.h"
}


template< class DictOptPlicy >
struct HillclimbAssertions {
    void RunFinalAssertions( const Key& gkey );
    void OnNewBest( const Key& gkey ) const;
};


template< class D >
void HillclimbAssertions< D >::OnNewBest( UNUSED const Key& gkey ) const {}
