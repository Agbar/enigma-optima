#include "error.h"

NO_RETURN
void err_illegal_char_fatal( UNUSED const char *s ) {
    exit( 2 );
}

NO_RETURN
void err_open_fatal_resume( UNUSED const char *s ) {
    exit( 3 );
}

NO_RETURN
void err_open_fatal( UNUSED const char *s ) {
    exit( 4 );
}

NO_RETURN
void err_input_fatal( UNUSED int type ) {
    exit( 5 );
}

extern inline
void exit_d( int errorCode UNUSED );
