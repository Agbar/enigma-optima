#include "global.h"
#include "key.h"

char
RingType_to_ALPHA( struct RingType rt ){
    return '0' + rt.type;
}

char
GreekRingType_to_ALPHA( struct GreekRingType rt ){
    return "BG"[rt.type-9];
}

char
UkwType_to_ALPHA( struct UkwType u ){
    return "ABCBC"[u.type];
}

/* initialize key to defaults */
int init_key_default( struct Key *const key, enum ModelType_t model )
{
    struct Key def_H  = { .slot={ { RingType_None }, { RingType_1 }, { RingType_2 }, { RingType_3 } },
                          .ukwnum.type = UkwType_B, 
                          .model=EnigmaModel_H  };
    struct Key def_M3 = { .slot={ { RingType_None }, { RingType_1 }, { RingType_2 }, { RingType_3 } },
                          .ukwnum.type = UkwType_B,
                          .model=EnigmaModel_M3 };
    struct Key def_M4 = { .slot={ { GreekRingType_Beta }, { RingType_1 }, { RingType_2 }, { RingType_3 } },
                          .ukwnum.type = UkwType_B_Thin,
                          .model=EnigmaModel_M4 };
    switch( model ) {
    case EnigmaModel_H :
        *key = def_H;
        break;
    case EnigmaModel_M3:
        *key = def_M3;
        break;
    case EnigmaModel_M4:
        *key = def_M4;
        break;
    default:
        return 0;
    }
    Fill0To25_echar(key->stbrett.letters);
    Fill0To25_echar( key->sf.map );
    return 1;
}

/* initializes each key element to the lowest possible value */
int init_key_low( struct Key *const key, enum ModelType_t model )
{
    struct Key low_H  = { .slot={ { RingType_None }, { RingType_1 }, { RingType_2 }, { RingType_3 } },
                          .ukwnum.type = UkwType_A,
                          .model=EnigmaModel_H  };
    struct Key low_M3 = { .slot={ { RingType_None }, { RingType_1 }, { RingType_2 }, { RingType_3 } },
                          .ukwnum.type = UkwType_B,
                          .model=EnigmaModel_M3 };
    struct Key low_M4 = { .slot={ { GreekRingType_Beta }, { RingType_1 }, { RingType_2 }, { RingType_3 } },
                          .ukwnum.type = UkwType_B_Thin,
                          .model=EnigmaModel_M4 };
    switch( model ) {
    case EnigmaModel_H :
        *key = low_H;
        break;
    case EnigmaModel_M3:
        *key = low_M3;
        break;
    case EnigmaModel_M4:
        *key = low_M4;
        break;
    default:
        return 0;
    }
    Fill0To25_echar(key->stbrett.letters);
    Fill0To25_echar( key->sf.map );
    return 1;
}

/* compares ukwnum thru r_mesg, omits g_ring, l_ring        */
/* returns -1 for k1 < k2, 0 for k1 == k2, 1 for k1 > k2    */
enum comparison_result
keycmp( const struct Key* k1, const struct Key* k2 )
{
  enum comparison_result cr;
  cr = UkwType_cmp( k1->ukwnum, k2->ukwnum );
  if( cr != cmp_equal ) return cr;
  if (   k1->slot.g.type != k2->slot.g.type ) {
    if ( k1->slot.g.type >  k2->slot.g.type ) return cmp_greater;
    else return cmp_less;
  }
  if (   k1->slot.l.type != k2->slot.l.type ) {
    if ( k1->slot.l.type >  k2->slot.l.type ) return cmp_greater;
    else return cmp_less;
  }
  if (   k1->slot.m.type != k2->slot.m.type ) {
    if ( k1->slot.m.type >  k2->slot.m.type ) return cmp_greater;
    else return cmp_less;
  }
  if (   k1->slot.r.type != k2->slot.r.type ) {
    if ( k1->slot.r.type >  k2->slot.r.type ) return cmp_greater;
    else return cmp_less;
  }
  cr = echar_delta_cmp( k1->ring.m, k2->ring.m );
  if( cr != cmp_equal ) return cr;
  cr = echar_delta_cmp( k1->ring.r, k2->ring.r );
  if( cr != cmp_equal ) return cr;
  cr = echar_delta_cmp( k1->mesg.g, k2->mesg.g );
  if( cr != cmp_equal ) return cr;
  cr = echar_delta_cmp( k1->mesg.l, k2->mesg.l );
  if( cr != cmp_equal ) return cr;
  cr = echar_delta_cmp( k1->mesg.m, k2->mesg.m );
  if( cr != cmp_equal ) return cr;
  cr = echar_delta_cmp( k1->mesg.r, k2->mesg.r );  
  return cr;
}


static inline void do_not_use_cmov() {
    asm volatile( "" ::: "memory" );
}


bool Key_equ( const struct Key* k1, const struct Key* k2 ) {
    const union {
        struct RingsState rs;
        uint32_t bits;
    } mesg1 = {.rs = k1->mesg},
      mesg2 = {.rs = k2->mesg};
    const union {
        struct Ringstellung rs;
        uint16_t bits;
    } ring1 = {.rs = k1->ring},
      ring2 = {.rs = k2->ring};
    if ( mesg1.bits != mesg2.bits) return false;
    do_not_use_cmov();
    if( ring1.bits != ring2.bits) return false;
    do_not_use_cmov();
    if( !RingTypes_equ( k1->slot, k2->slot ) ) return false;
    do_not_use_cmov();
    if( k1->ukwnum.type != k2->ukwnum.type) return false;
    return true;
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
