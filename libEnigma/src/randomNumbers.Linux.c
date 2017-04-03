#if defined(__linux__)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "randomNumbers.h"

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

#endif
