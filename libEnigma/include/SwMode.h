#pragma once


#include "global.h"


// SwMode is serialized in a resume file
struct SwMode {
    enum sw_mode_enum {
        SW_ONSTART,
        SW_OTHER,
        SW_ALL,
        SW_NONE,
        SINGLE_KEY,
        __SwMode__enforce_signed_type = -1,
    } mode;
};


PURE_FUNCTION
struct SwMode get_sw_mode( const char* s );
