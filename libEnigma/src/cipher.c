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

void init_path_lookup_H_M3(const Key *key, int len);
void init_path_lookup_ALL(const Key *key, int len);
void enigma_prepare_decoder_lookups(const Key* key, int len);

extern inline
text_t decode(size_t offset,size_t index, const PermutationMap_t* const stbrett);

/* Eintrittswalze */
text_t etw[52] =
     {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};


/* Walzen 1-8, B and G (M4): forward path */
text_t wal[11][78] = {

     /* null substitution for no greek wheel */
     {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25},

     {4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,23,20,18,15,0,8,1,17,2,9,
      4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,23,20,18,15,0,8,1,17,2,9,
      4,10,12,5,11,6,3,16,21,25,13,19,14,22,24,7,23,20,18,15,0,8,1,17,2,9},

     {0,9,3,10,18,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4,
      0,9,3,10,18,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4,
      0,9,3,10,18,8,17,20,23,1,11,7,22,19,12,2,16,6,25,13,15,24,5,21,14,4},

     {1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,12,20,18,16,14,
      1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,12,20,18,16,14,
      1,3,5,7,9,11,2,15,17,19,23,21,25,13,24,4,8,22,6,0,10,12,20,18,16,14},

     {4,18,14,21,15,25,9,0,24,16,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1,
      4,18,14,21,15,25,9,0,24,16,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1,
      4,18,14,21,15,25,9,0,24,16,20,8,17,7,23,11,13,5,19,6,10,3,2,12,22,1},

     {21,25,1,17,6,8,19,24,20,15,18,3,13,7,11,23,0,22,12,9,16,14,5,4,2,10,
      21,25,1,17,6,8,19,24,20,15,18,3,13,7,11,23,0,22,12,9,16,14,5,4,2,10,
      21,25,1,17,6,8,19,24,20,15,18,3,13,7,11,23,0,22,12,9,16,14,5,4,2,10},

     {9,15,6,21,14,20,12,5,24,16,1,4,13,7,25,17,3,10,0,18,23,11,8,2,19,22,
      9,15,6,21,14,20,12,5,24,16,1,4,13,7,25,17,3,10,0,18,23,11,8,2,19,22,
      9,15,6,21,14,20,12,5,24,16,1,4,13,7,25,17,3,10,0,18,23,11,8,2,19,22},

     {13,25,9,7,6,17,2,23,12,24,18,22,1,14,20,5,0,8,21,11,15,4,10,16,3,19,
      13,25,9,7,6,17,2,23,12,24,18,22,1,14,20,5,0,8,21,11,15,4,10,16,3,19,
      13,25,9,7,6,17,2,23,12,24,18,22,1,14,20,5,0,8,21,11,15,4,10,16,3,19},

     {5,10,16,7,19,11,23,14,2,1,9,18,15,3,25,17,0,12,4,22,13,8,20,24,6,21,
      5,10,16,7,19,11,23,14,2,1,9,18,15,3,25,17,0,12,4,22,13,8,20,24,6,21,
      5,10,16,7,19,11,23,14,2,1,9,18,15,3,25,17,0,12,4,22,13,8,20,24,6,21},

     {11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18,
      11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18,
      11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18},

     {5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3,
      5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3,
      5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3}

};


/* Umkehrwalzen A, B, C, B_THIN, C_THIN */
text_t ukw[5][52] = {

     {4,9,12,25,0,11,24,23,21,1,22,5,2,17,16,20,14,13,19,18,15,8,10,7,6,3,
      4,9,12,25,0,11,24,23,21,1,22,5,2,17,16,20,14,13,19,18,15,8,10,7,6,3},

     {24,17,20,7,16,18,11,3,15,23,13,6,14,10,12,8,4,1,5,25,2,22,21,9,0,19,
      24,17,20,7,16,18,11,3,15,23,13,6,14,10,12,8,4,1,5,25,2,22,21,9,0,19},

     {5,21,15,9,8,0,14,24,4,3,17,25,23,22,6,2,19,10,20,16,18,1,13,12,7,11,
      5,21,15,9,8,0,14,24,4,3,17,25,23,22,6,2,19,10,20,16,18,1,13,12,7,11},

     {4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18,
      4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18},

     {17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16,
      17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16}

};


/* Walzen 1-8, B and G (M4): reverse path */
text_t rev_wal[11][78] = {

     /* null substitution for no greek wheel */
     {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25},

     {20,22,24,6,0,3,5,15,21,25,1,4,2,10,12,19,7,23,18,11,17,8,13,16,14,9,
      20,22,24,6,0,3,5,15,21,25,1,4,2,10,12,19,7,23,18,11,17,8,13,16,14,9,
      20,22,24,6,0,3,5,15,21,25,1,4,2,10,12,19,7,23,18,11,17,8,13,16,14,9},

     {0,9,15,2,25,22,17,11,5,1,3,10,14,19,24,20,16,6,4,13,7,23,12,8,21,18,
      0,9,15,2,25,22,17,11,5,1,3,10,14,19,24,20,16,6,4,13,7,23,12,8,21,18,
      0,9,15,2,25,22,17,11,5,1,3,10,14,19,24,20,16,6,4,13,7,23,12,8,21,18},

     {19,0,6,1,15,2,18,3,16,4,20,5,21,13,25,7,24,8,23,9,22,11,17,10,14,12,
      19,0,6,1,15,2,18,3,16,4,20,5,21,13,25,7,24,8,23,9,22,11,17,10,14,12,
      19,0,6,1,15,2,18,3,16,4,20,5,21,13,25,7,24,8,23,9,22,11,17,10,14,12},

     {7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5,
      7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5,
      7,25,22,21,0,17,19,13,11,6,20,15,23,16,2,4,9,12,1,18,10,3,24,14,8,5},

     {16,2,24,11,23,22,4,13,5,19,25,14,18,12,21,9,20,3,10,6,8,0,17,15,7,1,
      16,2,24,11,23,22,4,13,5,19,25,14,18,12,21,9,20,3,10,6,8,0,17,15,7,1,
      16,2,24,11,23,22,4,13,5,19,25,14,18,12,21,9,20,3,10,6,8,0,17,15,7,1},

     {18,10,23,16,11,7,2,13,22,0,17,21,6,12,4,1,9,15,19,24,5,3,25,20,8,14,
      18,10,23,16,11,7,2,13,22,0,17,21,6,12,4,1,9,15,19,24,5,3,25,20,8,14,
      18,10,23,16,11,7,2,13,22,0,17,21,6,12,4,1,9,15,19,24,5,3,25,20,8,14},

     {16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1,
      16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1,
      16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1},

     {16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14,
      16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14,
      16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14},

     {17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19,
      17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19,
      17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19},

     {4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20,
      4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20,
      4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20}

};


/* Turnover points:  Walzen 1-5, Walzen 6-8 (/first/ turnover points) */
text_t wal_turn[9] = {0, 16, 4, 21, 9, 25, 12, 12, 12};

text_t path_lookup[CT][LAST_DIMENSION];

enigma_cipher_function_t enigma_cipher_decoder_lookup = {init_path_lookup_H_M3, init_path_lookup_ALL};

enigma_prepare_decoder_lookup_function_pt enigma_cipher_decoder_lookups_list[4];

void enigma_prepare_decoder_lookups(const Key* key, int len)
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
int scrambler_state(const Key *key, int len)
{
  int i;

  int m_slot = key->slot.m;
  int r_slot = key->slot.r;

  int l_ring = key->ring.l;
  int m_ring = key->ring.m;
  int r_ring = key->ring.r;
  int l_mesg = key->mesg.l;
  int m_mesg = key->mesg.m;
  int r_mesg = key->mesg.r;

  int l_offset, m_offset, r_offset;
  int m_turn, r_turn;
  int m_turn2 = -1, r_turn2 = -1;
  int p2 = 0, p3 = 0;


  /* calculate effective offset from ring and message settings */
  r_offset = (26 + r_mesg-r_ring) % 26;
  m_offset = (26 + m_mesg-m_ring) % 26;
  l_offset = (26 + l_mesg-l_ring) % 26;

  /* calculate turnover points from ring settings */
  r_turn = (26 + wal_turn[r_slot]-r_ring) % 26;
  m_turn = (26 + wal_turn[m_slot]-m_ring) % 26;

  /* second turnover points for wheels 6,7,8 */
  if (r_slot > 5)
    r_turn2 = (26 + 25-r_ring) % 26;
  if (m_slot > 5)
    m_turn2 = (26 + 25-m_ring) % 26;


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if (r_offset == r_turn || r_offset == r_turn2)
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if (m_offset == m_turn || m_offset == m_turn2) {
      p3 = 1;
      p2 = 1;
      if (i == 0)
        return SW_ONSTART;
      else
        return SW_OTHER;
    }

    r_offset++;
    r_offset %= 26;
    if (p2) {
      m_offset++;
      m_offset %= 26;
      p2 = 0;
    }
    if (p3) {
      l_offset++;
      l_offset %= 26;
      p3 = 0;
    }

  }

  return SW_NONE;

}

/* initialize lookup table for paths through scramblers, models H, M3 */
void init_path_lookup_H_M3(const Key *key, int len)
{
  int i, k;
  int c;

  int l_slot = key->slot.l;
  int m_slot = key->slot.m;
  int r_slot = key->slot.r;
  int l_ring = key->ring.l;
  int m_ring = key->ring.m;
  int r_ring = key->ring.r;
  int l_mesg = key->mesg.l;
  int m_mesg = key->mesg.m;
  int r_mesg = key->mesg.r;
  int ukwnum = key->ukwnum;

  int l_offset, m_offset, r_offset;
  int m_turn, r_turn;
  int m_turn2 = -1, r_turn2 = -1;
  int p2 = 0, p3 = 0;


  /* calculate effective offset from ring and message settings */
  r_offset = (26 + r_mesg-r_ring) % 26;
  m_offset = (26 + m_mesg-m_ring) % 26;
  l_offset = (26 + l_mesg-l_ring) % 26;

  /* calculate turnover points from ring settings */
  r_turn = (26 + wal_turn[r_slot]-r_ring) % 26;
  m_turn = (26 + wal_turn[m_slot]-m_ring) % 26;

  /* second turnover points for wheels 6,7,8 */
  if (r_slot > 5)
    r_turn2 = (26 + 25-r_ring) % 26;
  if (m_slot > 5)
    m_turn2 = (26 + 25-m_ring) % 26;


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if (r_offset == r_turn || r_offset == r_turn2)
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if (m_offset == m_turn || m_offset == m_turn2) {
      p3 = 1;
      p2 = 1;
    }

    r_offset++;
    r_offset %= 26;
    if (p2) {
      m_offset++;
      m_offset %= 26;
      p2 = 0;
    }
    if (p3) {
      l_offset++;
      l_offset %= 26;
      p3 = 0;
    }

    for (k = 0; k < 26; k++) {
      c = k;
      c = wal[r_slot][c+r_offset]-r_offset+26;
      c = wal[m_slot][c+m_offset]-m_offset+26;
      c = wal[l_slot][c+l_offset]-l_offset+26;
      c = ukw[ukwnum][c];
      c = rev_wal[l_slot][c+l_offset]-l_offset+26;
      c = rev_wal[m_slot][c+m_offset]-m_offset+26;
      c = rev_wal[r_slot][c+r_offset]-r_offset+26;
      c = etw[c];
      path_lookup[i][k] = c;
    }
  }
}


/* initialize lookup table for paths through scramblers, all models */
void init_path_lookup_ALL(const Key *key, int len)
{
  int i, k;
  text_t c;

  int ukwnum = key->ukwnum;
  int g_slot = key->slot.g;
  int l_slot = key->slot.l;
  int m_slot = key->slot.m;
  int r_slot = key->slot.r;
  int g_ring = key->ring.g;
  int l_ring = key->ring.l;
  int m_ring = key->ring.m;
  int r_ring = key->ring.r;
  int g_mesg = key->mesg.g;
  int l_mesg = key->mesg.l;
  int m_mesg = key->mesg.m;
  int r_mesg = key->mesg.r;

  int g_offset, l_offset, m_offset, r_offset;
  int m_turn, r_turn;
  int m_turn2 = -1, r_turn2 = -1;
  int p2 = 0, p3 = 0;

  /* calculate effective offset from ring and message settings */
  r_offset = (26 + r_mesg-r_ring) % 26;
  m_offset = (26 + m_mesg-m_ring) % 26;
  l_offset = (26 + l_mesg-l_ring) % 26;
  g_offset = (26 + g_mesg-g_ring) % 26;

  /* calculate turnover points from ring settings */
  r_turn = (26 + wal_turn[r_slot]-r_ring) % 26;
  m_turn = (26 + wal_turn[m_slot]-m_ring) % 26;

  /* second turnover points for wheels 6,7,8 */
  if (r_slot > 5)
    r_turn2 = (26 + 25-r_ring) % 26;
  if (m_slot > 5)
    m_turn2 = (26 + 25-m_ring) % 26;


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if (r_offset == r_turn || r_offset == r_turn2)
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if (m_offset == m_turn || m_offset == m_turn2) {
      p3 = 1;
      p2 = 1;
    }

    r_offset++;
    r_offset %= 26;
    if (p2) {
      m_offset++;
      m_offset %= 26;
      p2 = 0;
    }
    if (p3) {
      l_offset++;
      l_offset %= 26;
      p3 = 0;
    }

    for (k = 0; k < 26; k++) {
      c = k;
      c = wal[r_slot][c+r_offset]-r_offset+26;
      c = wal[m_slot][c+m_offset]-m_offset+26;
      c = wal[l_slot][c+l_offset]-l_offset+26;
      c = wal[g_slot][c+g_offset]-g_offset+26;
      c = ukw[ukwnum][c];
      c = rev_wal[g_slot][c+g_offset]-g_offset+26;
      c = rev_wal[l_slot][c+l_offset]-l_offset+26;
      c = rev_wal[m_slot][c+m_offset]-m_offset+26;
      c = rev_wal[r_slot][c+r_offset]-r_offset+26;
      c = etw[c];
      path_lookup[i][k] = c;
    }
  }

}


/* deciphers and calculates ic of plaintext, all models */
double dgetic_ALL(const Key *key, int len)
{
  int f[26] = {0};
  double S = 0;

  int i;
  text_t c;

  int ukwnum = key->ukwnum;
  int g_slot = key->slot.g;
  int l_slot = key->slot.l;
  int m_slot = key->slot.m;
  int r_slot = key->slot.r;
  int g_ring = key->ring.g;
  int l_ring = key->ring.l;
  int m_ring = key->ring.m;
  int r_ring = key->ring.r;
  int g_mesg = key->mesg.g;
  int l_mesg = key->mesg.l;
  int m_mesg = key->mesg.m;
  int r_mesg = key->mesg.r;

  int g_offset, l_offset, m_offset, r_offset;
  int m_turn, r_turn;
  int m_turn2 = -1, r_turn2 = -1;
  int p2 = 0, p3 = 0;


  if (len < 2)
    return 0;

  /* calculate effective offset from ring and message settings */
  r_offset = (26 + r_mesg-r_ring) % 26;
  m_offset = (26 + m_mesg-m_ring) % 26;
  l_offset = (26 + l_mesg-l_ring) % 26;
  g_offset = (26 + g_mesg-g_ring) % 26;

  /* calculate turnover points from ring settings */
  r_turn = (26 + wal_turn[r_slot]-r_ring) % 26;
  m_turn = (26 + wal_turn[m_slot]-m_ring) % 26;

  if (r_slot > 5)
    r_turn2 = (26 + 25-r_ring) % 26;
  if (m_slot > 5)
    m_turn2 = (26 + 25-m_ring) % 26;


  for (i = 0; i < len; i++) {

    /* determine if pawls are engaged */
    if (r_offset == r_turn || r_offset == r_turn2)
      p2 = 1;
    /* in reality pawl 3 steps both m_wheel and l_wheel */
    if (m_offset == m_turn || m_offset == m_turn2) {
      p3 = 1;
      p2 = 1;
    }

    r_offset++;
    r_offset %= 26;
    if (p2) {
      m_offset++;
      m_offset %= 26;
      p2 = 0;
    }
    if (p3) {
      l_offset++;
      l_offset %= 26;
      p3 = 0;
    }

    c = ciphertext.plain[i];  /* no plugboard */
    c = wal[r_slot][c+r_offset+26];
    c = wal[m_slot][c-r_offset+m_offset+26];
    c = wal[l_slot][c-m_offset+l_offset+26];
    c = wal[g_slot][c-l_offset+g_offset+26];
    c = ukw[ukwnum][c-g_offset+26];
    c = rev_wal[g_slot][c+g_offset+26];
    c = rev_wal[l_slot][c+l_offset-g_offset+26];
    c = rev_wal[m_slot][c+m_offset-l_offset+26];
    c = rev_wal[r_slot][c+r_offset-m_offset+26];
    c = etw[c-r_offset+26];
    f[c]++;

  }

  for (i = 0; i < 26; i++)
    S += f[i]*(f[i]-1);
  S /= len*(len-1);

  return S;

}

extern inline void Step1( int8_t* ringOffset );

extern inline void CalculatePermutationMap3Rotors( PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key );
extern inline void CalculatePermutationMap4Rotors( PermutationMap_t* const restrict map, struct RingsState rings, const Key* const restrict key );

extern inline void CopyRRing2Lookup( const Key* const restrict key, PermutationMap_t rRings[2] );
extern inline void StepAllRings( struct RingsState* const restrict rings, const struct Turnovers_t turns );
extern inline int8_t GetNextTurnover( const struct RingsState rings, const struct Turnovers_t turns );

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
