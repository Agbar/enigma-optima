#ifndef OS_WINDOWS_HEADRER_INCLUDED
#define OS_WINDOWS_HEADRER_INCLUDED

#include <signal.h>

extern volatile sig_atomic_t doShutdown;

void SetupProcessProperties( void ) ;

void InstallSighandler( void );

#endif
