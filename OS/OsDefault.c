/** \file
 * \brief This file implements OS related stuff for systems other than Windows.
 */

#include <signal.h>
#include <stdio.h>

#include "OS/Os.h"
#include "config/testing.h"
#include "error.h"

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
