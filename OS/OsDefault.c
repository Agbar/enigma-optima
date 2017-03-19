/** \file
 * \brief This file implements OS related stuff for systems other than Windows.
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/syscall.h>

#include "Os.h"
#include "config/testing.h"
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
    int seed;
    const size_t requestedBytesNum = 4;
    unsigned int ret = syscall( SYS_getrandom, &seed, requestedBytesNum, 0 );
    if ( ret == requestedBytesNum ) {
        return seed;
    } else {
        fputs( "enigma: error: Random seed generation failed\n", stderr );
        exit( ret );
    }
}
