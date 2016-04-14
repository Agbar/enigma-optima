#include <stdio.h>

#include "config/releaseVersion.h"
#include "cpu.h"
#include "global.h"

static char* isaAvx2  = "AVX2";
static char* isaAvx   = "AVX";
static char* isaSsse3 = "SSSE3";
static char* isaMmx   = "MMX";
static char* isaNone  = "no SIMD";

PURE_FUNCTION
char* cpuFlagsToString( enum _enigma_cpu_flags_t flags )
{
    if( flags & enigma_cpu_avx2 ){
        return isaAvx2;
    }
    else if( flags & enigma_cpu_avx ){
        return isaAvx;
    }
    else if( flags & enigma_cpu_ssse3 ){
        return isaSsse3;
    }
    else if ( flags & enigma_cpu_mmx ){
        return isaMmx;
    }
    else {
        return isaNone;
    }
}

void WriteStartupBanner( void ){
    fprintf(stderr,
            "Enigma Optima %s\n"
            "Best ISA: %s\n"
            , releaseVersion
            , cpuFlagsToString( enigma_cpu_flags ));
}
