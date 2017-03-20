#include "ModMath.h"


extern inline
int8_t SubMod26( int8_t a, int8_t b );

extern inline
int8_t AddMod26( int8_t a, int8_t b );

extern inline
void IncrementMod( int8_t* number, int8_t modulo );

extern inline
v16qi AddMod26_v16qi_int8( v16qi a, int8_t b );
extern inline
v16qi AddMod26_v16qi( v16qi a, v16qi b );
extern inline
v16qi SubMod26_v16qi_int8( v16qi a, int8_t b );
extern inline
v16qi SubMod26_v16qi( v16qi a, v16qi b );

extern inline
v32qi AddMod26_v32qi_int8( v32qi a, int8_t b );
extern inline
v32qi SubMod26_v32qi_int8( v32qi a, int8_t b );
extern inline
v32qi AddMod26_v32qi( v32qi a, v32qi b );
extern inline
v32qi SubMod26_v32qi( v32qi a, v32qi b );
