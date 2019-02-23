#include <stdlib.h>
#include "ciphertext.h"
#include "global.h"
#include "charmap.h"
#include "key.h"
#include "cpu.h"
#include "cipher.h"
#include "config/array_sizes.h"
#include "config/types.h"

#include "cipher_inlines.h"
#include "x86/cipherSsse3.h"
#include "x86/cipherAvx2.h"

void init_path_lookup_H_M3(const struct Key *key, int len);
void init_path_lookup_ALL(const struct Key *key, int len);
void enigma_prepare_decoder_lookups(const struct Key* key, int len);

/* null substitution for no greek wheel */
#define WALZ_0  (struct PermutationMap26) { .map = { {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19},{20},{21},{22},{23},{24},{25} } }
#define WALZ_1  (struct PermutationMap26) { .map = { {4},{10},{12},{5},{11},{6},{3},{16},{21},{25},{13},{19},{14},{22},{24},{7},{23},{20},{18},{15},{0},{8},{1},{17},{2},{9} } }
#define WALZ_2  (struct PermutationMap26) { .map = { {0},{9},{3},{10},{18},{8},{17},{20},{23},{1},{11},{7},{22},{19},{12},{2},{16},{6},{25},{13},{15},{24},{5},{21},{14},{4} } }
#define WALZ_3  (struct PermutationMap26) { .map = { {1},{3},{5},{7},{9},{11},{2},{15},{17},{19},{23},{21},{25},{13},{24},{4},{8},{22},{6},{0},{10},{12},{20},{18},{16},{14} } }
#define WALZ_4  (struct PermutationMap26) { .map = { {4},{18},{14},{21},{15},{25},{9},{0},{24},{16},{20},{8},{17},{7},{23},{11},{13},{5},{19},{6},{10},{3},{2},{12},{22},{1} } }
#define WALZ_5  (struct PermutationMap26) { .map = { {21},{25},{1},{17},{6},{8},{19},{24},{20},{15},{18},{3},{13},{7},{11},{23},{0},{22},{12},{9},{16},{14},{5},{4},{2},{10} } }
#define WALZ_6  (struct PermutationMap26) { .map = { {9},{15},{6},{21},{14},{20},{12},{5},{24},{16},{1},{4},{13},{7},{25},{17},{3},{10},{0},{18},{23},{11},{8},{2},{19},{22} } }
#define WALZ_7  (struct PermutationMap26) { .map = { {13},{25},{9},{7},{6},{17},{2},{23},{12},{24},{18},{22},{1},{14},{20},{5},{0},{8},{21},{11},{15},{4},{10},{16},{3},{19} } }
#define WALZ_8  (struct PermutationMap26) { .map = { {5},{10},{16},{7},{19},{11},{23},{14},{2},{1},{9},{18},{15},{3},{25},{17},{0},{12},{4},{22},{13},{8},{20},{24},{6},{21} } }
#define WALZ_9  (struct PermutationMap26) { .map = { {11},{4},{24},{9},{21},{2},{13},{8},{23},{22},{15},{1},{16},{12},{3},{17},{19},{0},{10},{25},{6},{5},{20},{7},{14},{18} } }
#define WALZ_10 (struct PermutationMap26) { .map = { {5},{18},{14},{10},{0},{13},{20},{4},{17},{7},{12},{1},{19},{8},{24},{2},{22},{11},{16},{15},{25},{23},{21},{6},{9},{3} } }
#define DOUBLE_WALZ_AT( i ) [(i)] = { .dbl = {WALZ_##i, WALZ_##i} }

/* Walzen 1-8, B and G (M4): forward path */
const union DoublePermutationMap wal[11] = {
    DOUBLE_WALZ_AT(0),
    DOUBLE_WALZ_AT(1),
    DOUBLE_WALZ_AT(2),
    DOUBLE_WALZ_AT(3),
    DOUBLE_WALZ_AT(4),
    DOUBLE_WALZ_AT(5),
    DOUBLE_WALZ_AT(6),
    DOUBLE_WALZ_AT(7),
    DOUBLE_WALZ_AT(8),
    DOUBLE_WALZ_AT(9),
    DOUBLE_WALZ_AT(10)
};

#define UMKEHRWALZEN_A (struct PermutationMap26) { .map = {{4},{9},{12},{25},{0},{11},{24},{23},{21},{1},{22},{5},{2},{17},{16},{20},{14},{13},{19},{18},{15},{8},{10},{7},{6},{3}} }
#define UMKEHRWALZEN_B (struct PermutationMap26) { .map = {{24},{17},{20},{7},{16},{18},{11},{3},{15},{23},{13},{6},{14},{10},{12},{8},{4},{1},{5},{25},{2},{22},{21},{9},{0},{19}} }
#define UMKEHRWALZEN_C (struct PermutationMap26) { .map = {{5},{21},{15},{9},{8},{0},{14},{24},{4},{3},{17},{25},{23},{22},{6},{2},{19},{10},{20},{16},{18},{1},{13},{12},{7},{11}} }
#define UMKEHRWALZEN_B_THIN (struct PermutationMap26) { .map = {{4},{13},{10},{16},{0},{20},{24},{22},{9},{8},{2},{14},{15},{1},{11},{12},{3},{23},{25},{21},{5},{19},{7},{17},{6},{18}} }
#define UMKEHRWALZEN_C_THIN (struct PermutationMap26) { .map = {{17},{3},{14},{1},{9},{13},{19},{10},{21},{4},{7},{12},{11},{5},{2},{22},{25},{0},{23},{6},{24},{8},{15},{18},{20},{16}} }
#define DOUBLE_UMKEHRWALZEN( name ) { .dbl = {UMKEHRWALZEN_##name, UMKEHRWALZEN_##name} }

/* Umkehrwalzen A, B, C, B_THIN, C_THIN */
const union DoublePermutationMap ukw[5] = {
    DOUBLE_UMKEHRWALZEN(A),
    DOUBLE_UMKEHRWALZEN(B),
    DOUBLE_UMKEHRWALZEN(C),
    DOUBLE_UMKEHRWALZEN(B_THIN),
    DOUBLE_UMKEHRWALZEN(C_THIN)
};

#define REV_WALZ_0  (struct PermutationMap26) { .map = { {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19},{20},{21},{22},{23},{24},{25} } }
#define REV_WALZ_1  (struct PermutationMap26) { .map = { {20},{22},{24},{6},{0},{3},{5},{15},{21},{25},{1},{4},{2},{10},{12},{19},{7},{23},{18},{11},{17},{8},{13},{16},{14},{9} } }
#define REV_WALZ_2  (struct PermutationMap26) { .map = { {0},{9},{15},{2},{25},{22},{17},{11},{5},{1},{3},{10},{14},{19},{24},{20},{16},{6},{4},{13},{7},{23},{12},{8},{21},{18} } }
#define REV_WALZ_3  (struct PermutationMap26) { .map = { {19},{0},{6},{1},{15},{2},{18},{3},{16},{4},{20},{5},{21},{13},{25},{7},{24},{8},{23},{9},{22},{11},{17},{10},{14},{12} } }
#define REV_WALZ_4  (struct PermutationMap26) { .map = { {7},{25},{22},{21},{0},{17},{19},{13},{11},{6},{20},{15},{23},{16},{2},{4},{9},{12},{1},{18},{10},{3},{24},{14},{8},{5} } }
#define REV_WALZ_5  (struct PermutationMap26) { .map = { {16},{2},{24},{11},{23},{22},{4},{13},{5},{19},{25},{14},{18},{12},{21},{9},{20},{3},{10},{6},{8},{0},{17},{15},{7},{1} } }
#define REV_WALZ_6  (struct PermutationMap26) { .map = { {18},{10},{23},{16},{11},{7},{2},{13},{22},{0},{17},{21},{6},{12},{4},{1},{9},{15},{19},{24},{5},{3},{25},{20},{8},{14} } }
#define REV_WALZ_7  (struct PermutationMap26) { .map = { {16},{12},{6},{24},{21},{15},{4},{3},{17},{2},{22},{19},{8},{0},{13},{20},{23},{5},{10},{25},{14},{18},{11},{7},{9},{1} } }
#define REV_WALZ_8  (struct PermutationMap26) { .map = { {16},{9},{8},{13},{18},{0},{24},{3},{21},{10},{1},{5},{17},{20},{7},{12},{2},{15},{11},{4},{22},{25},{19},{6},{23},{14} } }
#define REV_WALZ_9  (struct PermutationMap26) { .map = { {17},{11},{5},{14},{1},{21},{20},{23},{7},{3},{18},{0},{13},{6},{24},{10},{12},{15},{25},{16},{22},{4},{9},{8},{2},{19} } }
#define REV_WALZ_10 (struct PermutationMap26) { .map = { {4},{11},{15},{25},{7},{0},{23},{9},{13},{24},{3},{17},{10},{5},{2},{19},{18},{8},{1},{12},{6},{22},{16},{21},{14},{20} } }
#define DOUBLE_REV_WALZ_AT( i ) [(i)] = { .dbl = {REV_WALZ_##i, REV_WALZ_##i} }

/* Walzen 1-8, B and G (M4): reverse path */
const union DoublePermutationMap rev_wal[11] = {
    /* null substitution for no greek wheel */
    DOUBLE_REV_WALZ_AT(0),
    DOUBLE_REV_WALZ_AT(1),
    DOUBLE_REV_WALZ_AT(2),
    DOUBLE_REV_WALZ_AT(3),
    DOUBLE_REV_WALZ_AT(4),
    DOUBLE_REV_WALZ_AT(5),
    DOUBLE_REV_WALZ_AT(6),
    DOUBLE_REV_WALZ_AT(7),
    DOUBLE_REV_WALZ_AT(8),
    DOUBLE_REV_WALZ_AT(9),
    DOUBLE_REV_WALZ_AT(10)
};


/* Turnover points:  Walzen 1-5, Walzen 6-8 (/first/ turnover points) */
const struct turnover wal_turn[9] = {{0}, {16}, {4}, {21}, {9}, {25}, {12}, {12}, {12}};

union PermutationMap_t path_lookup[CT];

enigma_cipher_function_t enigma_cipher_decoder_lookup = {init_path_lookup_H_M3, init_path_lookup_ALL};

enigma_prepare_decoder_lookup_function_pt enigma_cipher_decoder_lookups_list[4];

void enigma_prepare_decoder_lookups(const struct Key* key, int len)
{
    enigma_prepare_decoder_lookup_function_pt *f = enigma_cipher_decoder_lookups_list;
    for(; *f ; f++)
    {
        (*f)(key,len);
    }
}

static
void CipherInit(enigma_cpu_flags_t cpu, enum ModelType_t machine_type, enigma_prepare_decoder_lookup_function_pt* cf ) {
    enigma_cipher_function_t* fun = &enigma_cipher_decoder_lookup;
    if( cpu & ( enigma_cpu_ssse3 | enigma_cpu_avx ) ) {
        fun = &enigma_cipher_decoder_lookup_ssse3;
    }
    if ( cpu & enigma_cpu_avx2 ) {
        fun = &enigma_cipher_DecoderLookupAvx2;
    }
    switch( machine_type ) {
    case EnigmaModel_H:
    case EnigmaModel_M3:
        *cf = fun->prepare_decoder_lookup_M_H3;
        return;
    case EnigmaModel_M4:
        *cf = fun->prepare_decoder_lookup_ALL;
        return;
    case EnigmaModel_Error:
        *cf = 0;// Hopefully causes exception early.
        return;
    }
}

void enigma_cipher_init(enigma_cpu_flags_t cpu, enum ModelType_t machine_type, enigma_prepare_decoder_lookup_function_pt* cf){
    CipherInit( cpu, machine_type, cf );
}

/* Check for slow wheel movement */
int scrambler_state(const struct Key* const key, int len)
{
  int i;

  struct RingType m_slot = key->slot.m;
  struct RingType r_slot = key->slot.r;

  struct echar_delta l_ring = key->ring.l;
  struct echar_delta m_ring = key->ring.m;
  struct echar_delta r_ring = key->ring.r;
  struct echar_delta l_mesg = key->mesg.l;
  struct echar_delta m_mesg = key->mesg.m;
  struct echar_delta r_mesg = key->mesg.r;

  struct echar_delta l_offset, m_offset, r_offset;
  struct turnover m_turn, r_turn;
  struct turnover m_turn2 = turnover_absent(), r_turn2 = turnover_absent();
  int p2 = 0, p3 = 0;


  /* calculate effective offset from ring and message settings */
  r_offset = echar_delta_sub( r_mesg, r_ring );
  m_offset = echar_delta_sub( m_mesg, m_ring );
  l_offset = echar_delta_sub( l_mesg, l_ring );

  /* calculate turnover points from ring settings */
  r_turn = turnover_sub_echar_delta( wal_turn[r_slot.type], r_ring );
  m_turn = turnover_sub_echar_delta( wal_turn[m_slot.type], m_ring );

  /* second turnover points for wheels 6,7,8 */
  if (r_slot.type > 5)
    r_turn2 = turnover_sub_echar_delta( turnover_second_notch(), r_ring );
  if (m_slot.type > 5)
    m_turn2 = turnover_sub_echar_delta( turnover_second_notch(), m_ring );


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if ( turnover_eq_echar_delta( r_turn, r_offset ) || turnover_eq_echar_delta( r_turn2, r_offset ) )
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if ( turnover_eq_echar_delta( m_turn, m_offset ) || turnover_eq_echar_delta( m_turn2, m_offset ) ) {
      p3 = 1;
      p2 = 1;
      if (i == 0)
        return SW_ONSTART;
      else
        return SW_OTHER;
    }

    echar_delta_rot_1( &r_offset );
    if (p2) {
      echar_delta_rot_1( &m_offset );
      p2 = 0;
    }
    if (p3) {
      echar_delta_rot_1( &l_offset );
      p3 = 0;
    }

  }

  return SW_NONE;

}

/* initialize lookup table for paths through scramblers, models H, M3 */
void init_path_lookup_H_M3(const struct Key* const key, int len)
{
  int i;

  struct RingType l_slot = key->slot.l;
  struct RingType m_slot = key->slot.m;
  struct RingType r_slot = key->slot.r;
  struct echar_delta l_ring = key->ring.l;
  struct echar_delta m_ring = key->ring.m;
  struct echar_delta r_ring = key->ring.r;
  struct echar_delta l_mesg = key->mesg.l;
  struct echar_delta m_mesg = key->mesg.m;
  struct echar_delta r_mesg = key->mesg.r;
  int ukwnum = key->ukwnum;

  struct turnover m_turn, r_turn;
  struct turnover m_turn2 = turnover_absent(), r_turn2 = turnover_absent();
  int p2 = 0, p3 = 0;


    /* calculate effective offset from ring and message settings */
    struct echar_delta
        r_offset = echar_delta_sub( r_mesg, r_ring ),
        m_offset = echar_delta_sub( m_mesg, m_ring ),
        l_offset = echar_delta_sub( l_mesg, l_ring );

  /* calculate turnover points from ring settings */
  r_turn = turnover_sub_echar_delta( wal_turn[r_slot.type], r_ring );
  m_turn = turnover_sub_echar_delta( wal_turn[m_slot.type], m_ring );

  /* second turnover points for wheels 6,7,8 */
  if (r_slot.type > 5)
    r_turn2 = turnover_sub_echar_delta( turnover_second_notch(), r_ring );
  if (m_slot.type > 5)
    m_turn2 = turnover_sub_echar_delta( turnover_second_notch(), m_ring );


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if ( turnover_eq_echar_delta( r_turn, r_offset ) || turnover_eq_echar_delta( r_turn2, r_offset) )
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if ( turnover_eq_echar_delta( m_turn, m_offset ) || turnover_eq_echar_delta( m_turn2, m_offset ) ) {
      p3 = 1;
      p2 = 1;
    }

    echar_delta_rot_1( &r_offset );
    if (p2) {
      echar_delta_rot_1( &m_offset );
      p2 = 0;
    }
    if (p3) {
      echar_delta_rot_1( &l_offset );
      p3 = 0;
    }

    const struct echar_delta 
        r_m_offset = echar_delta_sub( m_offset, r_offset ),
        m_l_offset = echar_delta_sub( l_offset, m_offset ),
        inv_l_offset = echar_delta_invert( l_offset ),
        l_m_offset = echar_delta_sub( m_offset, l_offset ),
        m_r_offset = echar_delta_sub( r_offset, m_offset );

    for (int k = 0; k < 26; k++) {
      struct echar c = make_echar( k );
      c = wal[r_slot.type].flat[ double_index( c,     r_offset ) ];
      c = wal[m_slot.type].flat[ double_index( c,   r_m_offset ) ];
      c = wal[l_slot.type].flat[ double_index( c,   m_l_offset ) ];
      c = ukw[ukwnum].flat[ double_index( c, inv_l_offset ) ];
      c = rev_wal[l_slot.type].flat[ double_index( c,   l_offset ) ];
      c = rev_wal[m_slot.type].flat[ double_index( c, l_m_offset ) ];
      c = rev_wal[r_slot.type].flat[ double_index( c, m_r_offset ) ];
      c = echar_sub_delta( c, r_offset );
      path_lookup[i].letters[k] = c;
    }
    FixPermutationMapTail( &path_lookup[i] );
  }
}


/* initialize lookup table for paths through scramblers, all models */
void init_path_lookup_ALL(const struct Key* const key, int len)
{
  int i;

  int ukwnum = key->ukwnum;
  struct RingTypes slot = key->slot;
  struct echar_delta g_ring = key->ring.g;
  struct echar_delta l_ring = key->ring.l;
  struct echar_delta m_ring = key->ring.m;
  struct echar_delta r_ring = key->ring.r;
  struct echar_delta g_mesg = key->mesg.g;
  struct echar_delta l_mesg = key->mesg.l;
  struct echar_delta m_mesg = key->mesg.m;
  struct echar_delta r_mesg = key->mesg.r;

  struct turnover m_turn, r_turn;
  struct turnover m_turn2 = turnover_absent(), r_turn2 = turnover_absent();
  int p2 = 0, p3 = 0;

    /* calculate effective offset from ring and message settings */
    struct echar_delta
      r_offset = echar_delta_sub( r_mesg, r_ring ),
      m_offset = echar_delta_sub( m_mesg, m_ring ),
      l_offset = echar_delta_sub( l_mesg, l_ring ),
      g_offset = echar_delta_sub( g_mesg, g_ring );

  /* calculate turnover points from ring settings */
  r_turn = turnover_sub_echar_delta( wal_turn[slot.r.type], r_ring );
  m_turn = turnover_sub_echar_delta( wal_turn[slot.m.type], m_ring );

  /* second turnover points for wheels 6,7,8 */
  if (slot.r.type > 5)
    r_turn2 = turnover_sub_echar_delta( turnover_second_notch(), r_ring );
  if (slot.m.type > 5)
    m_turn2 = turnover_sub_echar_delta( turnover_second_notch(), m_ring );


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if ( turnover_eq_echar_delta( r_turn, r_offset ) || turnover_eq_echar_delta( r_turn2, r_offset ))
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if ( turnover_eq_echar_delta( m_turn, m_offset ) || turnover_eq_echar_delta( m_turn2, m_offset )) {
      p3 = 1;
      p2 = 1;
    }

    echar_delta_rot_1( &r_offset );
    if (p2) {
      echar_delta_rot_1( &m_offset );
      p2 = 0;
    }
    if (p3) {
      echar_delta_rot_1( &l_offset );
      p3 = 0;
    }

    struct echar_delta 
        r_m_offset = echar_delta_sub( m_offset, r_offset ),
        m_l_offset = echar_delta_sub( l_offset, m_offset ),
        l_g_offset = echar_delta_sub( g_offset, l_offset ),
        inv_g_offset = echar_delta_invert( g_offset ),
        g_l_offset = echar_delta_sub( l_offset, g_offset ),
        l_m_offset = echar_delta_sub( m_offset, l_offset ),
        m_r_offset = echar_delta_sub( r_offset, m_offset );


    for (int k = 0; k < 26; k++) {
      struct echar c = make_echar( k );
      c = wal[slot.r.type].flat[ double_index( c,     r_offset ) ];
      c = wal[slot.m.type].flat[ double_index( c,   r_m_offset ) ];
      c = wal[slot.l.type].flat[ double_index( c,   m_l_offset ) ];
      c = wal[slot.g.type].flat[ double_index( c,   l_g_offset ) ];
      c = ukw[ukwnum].flat[ double_index( c, inv_g_offset ) ];
      c = rev_wal[slot.g.type].flat[ double_index( c,   g_offset ) ];
      c = rev_wal[slot.l.type].flat[ double_index( c, g_l_offset ) ];
      c = rev_wal[slot.m.type].flat[ double_index( c, l_m_offset ) ];
      c = rev_wal[slot.r.type].flat[ double_index( c, m_r_offset ) ];
      c = echar_sub_delta( c, r_offset );
      path_lookup[i].letters[k] = c;
    }
    FixPermutationMapTail( &path_lookup[i] );
  }

}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
