#include <stdio.h>

#include "error.h"
#include "resume_out.h"
#include "state.h"

void save_state( const struct State* state )
{
    FILE *fp = fopen( "00hc.resume", "w" );
    if ( fp  == NULL ){
        err_open_fatal( "00hc.resume" );
    }
    print_state( fp, state );
    if ( ferror( fp ) != 0 ){
        err_stream_fatal( "00hc.resume" );
    }
    fclose(fp);
}
