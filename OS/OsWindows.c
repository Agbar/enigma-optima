/** \file
 *  \brief Windows specific things and stuff.
 *  Compile and link this module only for Windows platform.
 */

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "Os.h"
#include "..\error.h"
#include "..\config\testing.h"

volatile sig_atomic_t doShutdown;

void SetupProcessProperties( void ) {
#if defined LOW_PRIORITY
    // set thread priority
    HANDLE currThing = GetCurrentThread();
    /* THREAD_PRIORITY_IDLE gives base priority 1 in all situations except when
     * process is in REALTIME_PRIORITY_CLASS.
     * see: http://msdn.microsoft.com/en-us/library/windows/desktop/ms685100.aspx */
    if( SetThreadPriority( currThing, THREAD_PRIORITY_IDLE ) == 0 ) {
        fputs( "enigma: warning: could not set thread priority to idle\n", stderr );
    }
    // Disable (sic!) priority boost.
    if( !SetThreadPriorityBoost( currThing, TRUE ) ){
        fputs( "enigma: warning: could not disable thread priority boost\n", stderr );
    }
#endif
}

void handle_signal(int signum __attribute__ ((unused)))
{
    doShutdown = 1;
}

void InstallSighandler( void ) {
    if( signal( SIGINT, handle_signal ) == SIG_ERR ) {
        err_sigaction_fatal( SIGINT );
    }
    if( signal( SIGTERM, handle_signal ) == SIG_ERR ) {
        err_sigaction_fatal( SIGTERM );
    }
}

void SetupRandomGenerator( void ) {
    unsigned int seed;
#if !defined CONSTANT_SEED
    seed = time( NULL );
#else
    seed = CONSTANT_SEED_VALUE;
#endif
    srand( seed );
}

int GetRandomNumber( void ) {
    return rand();
}
