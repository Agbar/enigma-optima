#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

extern "C" {
#include "key.h"
}

namespace heavy_test {


struct ExpectedRings {
    UkwType ukw;
    RingTypes slot;
};


struct ExpectedKeyState {
    int32_t score;
    std::string stecker;
    std::string ring;
    std::string mesg;
};


void AssertMatch( const Key& actual_key,
                  const ExpectedKeyState& x_state,
                  const ExpectedRings& x_rings );


RingTypes operator"" _ringTypes( const char*, std::size_t );


constexpr UkwType operator"" _ukw( const char v ) {
    return UkwType{
        v == 'A'
            ? UkwType_A
            : v == 'B'
                  ? UkwType_B
                  : v == 'C'
                        ? UkwType_C
                        : throw std::logic_error( "Unexpected UKW literal." )};
}

} // namespace heavy_test
