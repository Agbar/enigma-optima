#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "randomNumbers.h"

int GetRandomNumber( void ) {
    return rand();
}

void SetupRandomGenerator( void ) {
    unsigned int seed;
#if !defined CONSTANT_SEED
    seed = GetRSeed();
#else
    seed = CONSTANT_SEED_VALUE;
#endif
    srand( seed );
    fprintf( stderr, "Seed set to: %u.\n", seed );
}
