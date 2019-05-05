#ifndef MOD_MATH_H_INCLUDED
#define MOD_MATH_H_INCLUDED

#include <inttypes.h>
#include "global.h"
#include "config/types.h"

/* int8_t
 * * * * * */
/** \brief Subtract modulo 26.
 * Computes difference modulo 26 of given parameters.
 * \param a int8_t Minuend
 * \param b int8_t Subtrahend
 * \return int8_t Difference modulo 26
 */
CONST_FUNCTION
static inline
int8_t SubMod26( int8_t a, int8_t b )
{
    int8_t value = a - b;
    if( value < 0 ) value += 26;
    return value;
}

/** \brief Add modulo 26.
 * Computes difference modulo 26 of given parameters.
 * \param a int8_t Addend.
 * \param b int8_t Addend.
 * \return int8_t  Sum modulo 26.
 */
CONST_FUNCTION
static inline
int8_t AddMod26( int8_t a, int8_t b )
{
    int8_t value = a + b;
    if( value >= 26 ) value -= 26;
    return value;
}

static inline
void IncrementMod(int8_t* number, int8_t modulo)
{
    if( ++*number == modulo  ) *number = 0;
}

static inline
void IncrementModU(uint8_t* number, uint8_t modulo)
{
    if( ++*number == modulo  ) *number = 0;
}

/* v16qi
 * * * * * */
CONST_FUNCTION
static inline
v16qu AddMod26_v16qu_uint8( v16qu a, uint8_t b ){
    v16qu value = a + b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST_FUNCTION
static inline
v16qs AddMod26_v16qs_v16qu( v16qs a, v16qu b ){
    v16qs value = a + (v16qs)b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST_FUNCTION
static inline
v16qi SubMod26_v16qi_int8( v16qi a, int8_t b ){
    v16qi value = a - b;
    value += ( value < 0 ) & 26;
    return value;
}

CONST_FUNCTION
static inline
v16qs SubMod26_v16qs_v16qu( v16qs a, v16qu b ){
    v16qs value = a - (v16qs) b;
    value += ( value < 0 ) & 26;
    return value;
}

/* v32qi
 * * * * * */
CONST_FUNCTION
static inline
v32qu AddMod26_v32qu_uint8( v32qu a, uint8_t b ){
    v32qu value = a + b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST_FUNCTION
static inline
v32qs AddMod26_v32qs_v32qu( v32qs a, v32qu b ){
    v32qs value = a + (v32qs)b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST_FUNCTION
static inline
v32qi SubMod26_v32qi_int8( v32qi a, int8_t b ){
    v32qi value = a - b;
    value += ( value < 0 ) & 26;
    return value;
}

CONST_FUNCTION
static inline
v32qs SubMod26_v32qs_v32qu( v32qs a, v32qu b ){
    v32qs value = a - (v32qs)b;
    value += ( value < 0 ) & 26;
    return value;
}

#endif
