#ifndef ENIGMA_CPU_H_INCLUDED
#define ENIGMA_CPU_H_INCLUDED

typedef enum _enigma_cpu_flags_t
{
    enigma_cpu_generic = 0,
    enigma_cpu_mmx     = 1,
    enigma_cpu_ssse3   = 2,
    enigma_cpu_avx     = 4,
    enigma_cpu_avx2    = 8,
    enigma_cpu_all     = ~0,
} enigma_cpu_flags_t;

extern enigma_cpu_flags_t enigma_cpu_flags;

void enigma_cpu_flags_init(enigma_cpu_flags_t allowed);

#endif
