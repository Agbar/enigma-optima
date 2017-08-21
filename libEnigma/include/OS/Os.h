#pragma once

#include <signal.h>

extern volatile sig_atomic_t doShutdown;

void SetupProcessProperties( void ) ;

void InstallSighandler( void );
