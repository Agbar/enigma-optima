#include "scoreSimple.h"
#include "score.h"
#include "ciphertext.h"
#include "dict.h"
#include "cipher.h"


double icscoreSimple(const Key* const restrict key, scoreLength_t len);
int   uniscoreSimple(const Key* const restrict key, scoreLength_t len);
int    biscoreSimple(const Key* const restrict key, scoreLength_t len);
int   triscoreSimple(const Key* const restrict key, scoreLength_t len);

enigma_score_function_t enigmaScoreSimple = { triscoreSimple, biscoreSimple, icscoreSimple, uniscoreSimple};

PURE_FUNCTION
double icscoreSimple(const Key* const restrict key, scoreLength_t len)
{
  int f[26] = {0};
  double S = 0;
  int i;
  int c;

  if (len < 2)
    return 0;

  const PermutationMap_t* const stbrett = &key->stbrett;

  for (i = 0; i < len; i++) {
    c = decode(0,i,stbrett);
    f[c]++;
  }

  for (i = 0; i < 26; i++)
    S += f[i]*(f[i]-1);

  return S;
}

PURE_FUNCTION
int uniscoreSimple(const Key* const restrict key, scoreLength_t len)
{
  int i;
  int c;
  int s = 0;
  const PermutationMap_t* const stbrett = &key->stbrett;

  for (i = 0; i < len; i++) {
    c = decode(0,i,stbrett);
    s += unidict[c];
  }

  return s;
}

PURE_FUNCTION
int biscoreSimple(const Key* const restrict key, scoreLength_t len)
{
  int i;
  int c1, c2;
  int s = 0;
  const PermutationMap_t* const stbrett = &key->stbrett;

  c1 = decode(0,0,stbrett);

  for (i = 1; i < len; i++) {
    c2 = decode(0,i,stbrett);
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

PURE_FUNCTION
int triscoreSimple(const Key* const restrict key,  scoreLength_t len)
{
  int i;
  int c1, c2, c3;
  int s = 0;
  const PermutationMap_t* const stbrett = &key->stbrett;

  c1 = decode(0,0,stbrett);

  c2 = decode(1,0,stbrett);

  for (i = 2; i < len; i++) {
    c3 = decode(0,i,stbrett);
    s += tridict[c1][c2][c3];

    c1 = c2;
    c2 = c3;
  }

  return s;
}

