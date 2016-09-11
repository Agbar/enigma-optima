#pragma once

#ifndef SIMPLESCORE
//# define SIMPLESCORE
#endif

#ifndef TESTING
//# define TESTING
#endif

#if defined TESTING || defined PROFILING
# define CONSTANT_SEED
#endif

#if !defined PROFILING && !defined TESTING && !defined DEBUG
#  define  LOW_PRIORITY
#endif

// set default value for CONSTANT_SEED_VALUE
#if !defined CONSTANT_SEED_VALUE
# define CONSTANT_SEED_VALUE 315
#endif

#ifndef ENIGMA_CPU_SPECIFIC
//#define ENIGMA_CPU_SPECIFIC enigma_cpu_generic
#endif
