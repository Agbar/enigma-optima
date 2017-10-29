#include "error.h"

NO_RETURN
void err_illegal_char_fatal( UNUSED const char *s ) {
    exit( 1 );
}

NO_RETURN
void err_open_fatal_resume( UNUSED const char *s ) {
    exit( 1 );
}

NO_RETURN
void err_open_fatal( UNUSED const char *s ) {
    exit( 1 );
}

NO_RETURN
void err_input_fatal( UNUSED int type ) {
    exit( 1 );
}

extern inline
void exit_d( int errorCode UNUSED );
