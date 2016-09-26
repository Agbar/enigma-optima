/** \file
 * \brief This file implements OS related stuff for systems other than Windows.
 */
#include <signal.h>
#include <sys/time.h>

#include "Os.h"
#include "../config/testing.h"
#include "../error.h"

// objects
struct sigaction sigact;
volatile sig_atomic_t doShutdown;

void SetupProcessProperties( void ) {

}

void handle_signal(int signum __attribute__ ((unused))) {
    doShutdown = 1;
}

void InstallSighandler( void ) {
    if( sigaction( SIGINT, &sigact, NULL ) != 0 ) {
        err_sigaction_fatal( SIGINT );
    }
    if( sigaction( SIGQUIT, &sigact, NULL ) != 0 ) {
        err_sigaction_fatal( SIGQUIT );
    }
    if( sigaction( SIGTERM, &sigact, NULL ) != 0 ) {
        err_sigaction_fatal( SIGTERM );
    }
}

unsigned int GetRSeed( void ) {
    struct timeval tv;
    gettimeofday( &tv, NULL );
    return  ( tv.tv_sec % 1000 ) * 1000000 + tv.tv_usec;
}
