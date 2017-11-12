#pragma once

#include <benchmark/benchmark.h>

extern "C" {
#include "key.h"
}

struct MessageAndKeyBasedFixture
    : benchmark::Fixture {
    size_t len = 0;
    Key key {};
};
