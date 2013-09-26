#ifndef MOD_MATH_H_INCLUDED
#define MOD_MATH_H_INCLUDED

#include <inttypes.h>
#include "config/types.h"

#define CONST __attribute__ ((const))

/* int8_t
 * * * * * */
/** \brief Subtract modulo 26.
 * Computes difference modulo 26 of given parameters.
 * \param a int8_t Minuend
 * \param b int8_t Subtrahend
 * \return int8_t Difference modulo 26
 */
CONST
inline
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
CONST
inline
int8_t AddMod26( int8_t a, int8_t b )
{
    int8_t value = a + b;
    if( value >= 26 ) value -= 26;
    return value;
}

inline
void IncrementMod(int8_t* number, int8_t modulo)
{
    if( ++*number == modulo  ) *number = 0;
}

/* v16qi
 * * * * * */
CONST
inline
v16qi AddMod26_v16qi_int8( v16qi a, int8_t b ){
    v16qi value = a + b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST
inline
v16qi AddMod26_v16qi( v16qi a, v16qi b ){
    v16qi value = a + b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST
inline
v16qi SubMod26_v16qi_int8( v16qi a, int8_t b ){
    v16qi value = a - b;
    value += ( value < 0 ) & 26;
    return value;
}

CONST
inline
v16qi SubMod26_v16qi( v16qi a, v16qi b ){
    v16qi value = a - b;
    value += ( value < 0 ) & 26;
    return value;
}

/* v32qi
 * * * * * */
CONST
inline
v32qi AddMod26_v32qi_int8( v32qi a, int8_t b ){
    v32qi value = a + b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST
inline
v32qi AddMod26_v32qi( v32qi a, v32qi b ){
    v32qi value = a + b;
    value -= ( value >= 26 ) & 26;
    return value;
}

CONST
inline
v32qi SubMod26_v32qi_int8( v32qi a, int8_t b ){
    v32qi value = a - b;
    value += ( value < 0 ) & 26;
    return value;
}

CONST
inline
v32qi SubMod26_v32qi( v32qi a, v32qi b ){
    v32qi value = a - b;
    value += ( value < 0 ) & 26;
    return value;
}

#endif
