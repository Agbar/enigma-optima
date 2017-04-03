#if defined(_WIN32)

// enable rand_s function
#define _CRT_RAND_S

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

#include "randomNumbers.h"

unsigned int GetRSeed( void ) {
    uint32_t randomValue;
    errno_t isError = rand_s( &randomValue );
    if( !isError ) {
        return randomValue;
    }
    else {
        fputs("enigma: error: Random seed generation failed\n", stderr);
        exit( isError );
    }
}

#endif
