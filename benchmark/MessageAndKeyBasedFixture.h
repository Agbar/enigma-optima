#pragma once

#include <benchmark/benchmark.h>

#include "AlignedAllocationTrait.h"

extern "C" {
#include "key.h"
}

struct MessageAndKeyBasedFixture
    : benchmark::Fixture
    , public AlignedAllocationTrait<MessageAndKeyBasedFixture> {
    size_t len = 0;
    Key key {};
};
