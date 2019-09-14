#include "scoreSimple.h"
#include "score.h"
#include "ciphertext.h"
#include "dict.h"
#include "cipher.h"


int      icscoreSimple( const struct Key* const restrict key, scoreLength_t len );
int     uniscoreSimple( const struct Key* const restrict key, scoreLength_t len );
int      biscoreSimple( const struct Key* const restrict key, scoreLength_t len );
int     triscoreSimple( const struct Key* const restrict key, scoreLength_t len );

enigma_score_function_t enigmaScoreSimple = { triscoreSimple, biscoreSimple, icscoreSimple, uniscoreSimple};

PURE_FUNCTION
int icscoreSimple( const struct Key* const restrict key, scoreLength_t len )
{
  int f[26] = {0};  
  int i;

  if (len < 2)
    return 0;

  const union PermutationMap_t* const stbrett = &key->stbrett;

  for (i = 0; i < len; i++) {
    struct echar c = decode(0,i,stbrett);
    f[ echar_0_based_index( c ) ]++;
  }

  uint16_t S = 0;
  for (i = 0; i < 26; i++)
    S += f[i]*(f[i]-1);

  return S;
}

PURE_FUNCTION
int uniscoreSimple(const struct Key* const restrict key, scoreLength_t len)
{
  int i;
  int s = 0;
  const union PermutationMap_t* const stbrett = &key->stbrett;

  for (i = 0; i < len; i++) {
    struct echar c = decode(0,i,stbrett);
    s += unidict[ echar_0_based_index( c ) ];
  }

  return s;
}

PURE_FUNCTION
int biscoreSimple(const struct Key* const restrict key, scoreLength_t len)
{
  int i;
  int s = 0;
  const union PermutationMap_t* const stbrett = &key->stbrett;

  struct echar c1 = decode(0,0,stbrett);

  for (i = 1; i < len; i++) {
    struct echar c2 = decode(0,i,stbrett);
    s += bidict[ echar_0_based_index( c1 ) ]
               [ echar_0_based_index( c2 ) ];
    c1 = c2;
  }

  return s;

}

PURE_FUNCTION
int triscoreSimple(const struct Key* const restrict key,  scoreLength_t len)
{
  int i;
  int s = 0;
  const union PermutationMap_t* const stbrett = &key->stbrett;

  struct echar c1 = decode(0,0,stbrett);

  struct echar c2 = decode(1,0,stbrett);

  for (i = 2; i < len; i++) {
    struct echar c3 = decode(0,i,stbrett);
    s += tridict[ echar_0_based_index( c1 ) ]
                [ echar_0_based_index( c2 ) ]
                [ echar_0_based_index( c3 ) ];

    c1 = c2;
    c2 = c3;
  }

  return s;
}

