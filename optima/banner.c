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

static int pointerSize = 8 * __SIZEOF_POINTER__;

#ifdef _WIN32
# define OS_NAME "Windows"
#elif defined(__linux__)
# define OS_NAME "Linux"
#endif

static char* osName = OS_NAME;

void WriteStartupBanner( void ){
    fprintf(stderr,
            "Enigma Optima %s %s%d (%s)\n"
            "Best ISA: %s\n"
            , releaseVersion
            , osName
            , pointerSize
            , compilerVersion
            , cpuFlagsToString( enigma_cpu_flags ));
}
