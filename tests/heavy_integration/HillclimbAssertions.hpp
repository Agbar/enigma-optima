#pragma once


extern "C" {
#include "key.h"
}


template< class DictOptPlicy >
struct HillclimbAssertions {
    void RunFinalAssertions( const Key& gkey );
};
