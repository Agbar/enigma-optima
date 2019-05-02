#pragma once

#include <stdint.h>

struct turnover
{
    uint8_t notch;
};

static inline
struct turnover
turnover_absent() {
    return (struct turnover) { .notch = UINT8_MAX };
}

static inline
struct turnover
turnover_second_notch(){
    return (struct turnover) { .notch = 25 };
}
