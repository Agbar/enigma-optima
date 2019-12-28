#include <exception>

extern "C" {
#include "error.h"
#include "global.h"
}


void err_input_fatal( UNUSED int type ) {
    throw std::exception();
}
