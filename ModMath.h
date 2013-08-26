#ifndef MOD_MATH_H_INCLUDED
#define MOD_MATH_H_INCLUDED

#include <inttypes.h>


/** \brief Subtract modulo 26.
 * Computes difference modulo 26 of given parameters.
 * \param a int8_t Minuend
 * \param b int8_t Subtrahend
 * \return int8_t Difference modulo 26
 */
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

#endif
