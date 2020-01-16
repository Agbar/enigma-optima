#pragma once

#include <cstdint>
#include <string>

extern "C" {
#include "key.h"
}

namespace heavy_test {

struct ExpectedKey {
    int32_t score;
    UkwType ukw;
    RingTypes slot;
    std::string stecker;
    std::string ring;
    std::string mesg;

    void AssertMatch( const Key& gkey ) const;
};

RingTypes operator"" _ringTypes( const char*, std::size_t );

} // namespace heavy_test
