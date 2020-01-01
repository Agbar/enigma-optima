#include "error.h"

NO_RETURN
void err_input_fatal( UNUSED int type ) {
    exit( 5 );
}

extern inline
void exit_d( int errorCode UNUSED );
