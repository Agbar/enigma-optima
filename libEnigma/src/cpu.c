#include <cpuid.h>
#include "cpu.h"

#include "config/testing.h"

enigma_cpu_flags_t enigma_cpu_flags;

static enigma_cpu_flags_t get_hardware_supported_cpu_flags(void);

void enigma_cpu_flags_init(enigma_cpu_flags_t allowed)
{
#ifndef ENIGMA_CPU_SPECIFIC
    enigma_cpu_flags_t cpu_flags = get_hardware_supported_cpu_flags();

    cpu_flags &= allowed;
    enigma_cpu_flags = cpu_flags;
#else
    enigma_cpu_flags = ENIGMA_CPU_SPECIFIC;
#endif
}

static enigma_cpu_flags_t get_hardware_supported_cpu_flags(void)
{
    unsigned int eax,
                 ebx = 0,
                 ecx = 0,
                 edx = 0;
    enigma_cpu_flags_t cpu_flags = enigma_cpu_generic;

    __get_cpuid( 1, &eax, &ebx, &ecx, &edx );

    if ( edx & bit_MMX ){
        cpu_flags |= enigma_cpu_mmx;
    }

    if( ecx & bit_SSSE3 ){
        cpu_flags |= enigma_cpu_ssse3;
    }

    if( ecx & ( bit_AVX | bit_OSXSAVE) )
    {
        // confirm OS support for AVX
        ecx =0;
        asm("xgetbv" : "=d"(edx) , "=a"(eax) : "c" (ecx) );

        if ( ( eax & 0x6) == 0x6 ) //enabled state support for XMM and YMM registers
        {
            cpu_flags |= enigma_cpu_avx;

            // Use _macro_ from cpuid.h as it has 2 parameters.
            __cpuid_count( 7, 0, eax, ebx, ecx, edx );

            if( ebx & bit_AVX2 ) {
                cpu_flags |= enigma_cpu_avx2;
            }
        }
    }
    return cpu_flags;
}
