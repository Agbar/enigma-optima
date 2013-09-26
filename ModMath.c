#include "ModMath.h"


extern
int8_t SubMod26( int8_t a, int8_t b );

extern
int8_t AddMod26( int8_t a, int8_t b );

extern
void IncrementMod( int8_t* number, int8_t modulo );

extern
v16qi AddMod26_v16qi_int8( v16qi a, int8_t b );
extern
v16qi SubMod26_v16qi_int8( v16qi a, int8_t b );
