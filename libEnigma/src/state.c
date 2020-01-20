#include <stdio.h>
#include <time.h>

#include "error.h"
#include "resume_out.h"
#include "state.h"

time_t last_save_state = 0;

void save_state( const struct State* state, bool force_save ) {
    time_t prev_save = last_save_state;
    last_save_state = time( NULL );
    if( !force_save ) {
        if( !prev_save ) return; // do not save on first call
        if( difftime( last_save_state, prev_save ) < 120. /*seconds*/ ) return;
    }
    FILE* fp = fopen( "00hc.resume", "w" );
    if( fp == NULL ) {
        err_open_fatal( "00hc.resume" );
    }
    print_state( fp, state );
    if( ferror( fp ) != 0 ) {
        err_stream_fatal( "00hc.resume" );
    }
    fclose( fp );
}
