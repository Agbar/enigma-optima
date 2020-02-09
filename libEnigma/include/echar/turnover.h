#pragma once

#include <stdbool.h>

#include "echar_delta_data.h"
#include "turnover_data.h"

static inline
bool
turnover_eq_absent( struct turnover t ){
    return t.notch == turnover_absent().notch;
}

static inline
bool turnover_eq_echar_delta( struct turnover t, struct echar_delta ed ){
    return t.notch == ed.delta;
}

static inline
struct turnover
turnover_sub_echar_delta( struct turnover t, struct echar_delta ed ){
    if(turnover_eq_absent( t )) return t;
    int8_t s = (int8_t)t.notch - ed.delta;
    if( s < 0 ) s += 26;
    return (struct turnover){ .notch = (uint8_t)s };
}

static inline
struct turnover
turnover_select_next( struct echar_delta ed, struct turnover t1, struct turnover t2 ){
    if( ed.delta <= t1.notch )
    {
        return t1;
    }
    else if( (int8_t)ed.delta <= (int8_t)t2.notch )
    {
        return t2;
    }
    else
    {
        return t1;
    }
}
