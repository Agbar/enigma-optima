#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <stdint.h>

typedef uint32_t dict_t;

// In general on every processor after Pentium Pro MOVZX is as fast as MOV.
// At most one cycle more when load is from memory on AMDs.
typedef int8_t text_t;



// vectors
typedef unsigned char v16qu __attribute__ ((vector_size(16)));
typedef   signed char v16qi __attribute__ ((vector_size(16)));

typedef unsigned char v32qu __attribute__ ((vector_size(32)));
typedef   signed char v32qi __attribute__ ((vector_size(32)));


#endif // CONFIG_TYPES_H
