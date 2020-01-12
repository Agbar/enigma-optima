#pragma once


extern "C" {
#include "key.h"
}


template< class DictOptPlicy >
struct Assertions {
    void RunFinalAssertions( const Key& gkey );
};
