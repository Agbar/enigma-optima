#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <stdint.h>

typedef uint32_t dict_t;

// In general on every processor after Pentium Pro MOVZX is as fast as MOV.
// At most one cycle more when load is from memory on AMDs.
typedef int8_t text_t;

/* vectors
Naming guide for GCC vector naming scheme. (I have not found it elsewhere).
Type name is built like: vNNlt.
NN: number of elements
l: length of element, coded as:
               bits type                asm
    q: quarter   8  char                byte
    h: half     16  short               word
    s: single   32  int, float          double word
    d: double   64  long long, double   quad word
t: type
    i: integer (default)
    f: floating point
I extend type to allow:
    s: signed
    u: unsigned
as sometimes it is convenient ie. unsigned right shuffles, etc.
 * * * * * * * * * * */
/** \brief Vector of 16 unsigned bytes. 128 bit.
 */
typedef unsigned char   v16qu   __attribute__ ((vector_size(16)));

/** \brief Vector of 16 bytes. 128 bit, default signedness.
 */
typedef char            v16qi   __attribute__ ((vector_size(16)));

/** \brief Vector of 8 words. 128 bit, default signedness.
 */
typedef short           v8hi    __attribute__ ((vector_size(16)));

/** \brief Vector of 2 quadwords. 128 bit, defauld signedness.
 */
typedef long long       v2di    __attribute__ ((vector_size(16)));

/** \brief Vector of 32 unsigned bytes. 256 bit.
 */
typedef unsigned char   v32qu   __attribute__ ((vector_size(32)));

/** \brief vector of 32 bytes. 256 bit, default signedness.
 */
typedef char            v32qi   __attribute__ ((vector_size(32)));

/** \brief Vector of 4 double-quadwords. 256 bit. */
typedef long long int   v4di    __attribute__ ((vector_size(32)));

#endif // CONFIG_TYPES_H
