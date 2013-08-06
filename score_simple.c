#include "score_simple.h"
#include "score.h"
#include "ciphertext.h"
#include "dict.h"
#include "cipher.h"


double icscore_simple(const decode_mapping_t* stbrett, int len);
int   uniscore_simple(const decode_mapping_t* stbrett, int len);
int    biscore_simple(const decode_mapping_t* stbrett, int len);
int   triscore_simple(const decode_mapping_t* stbrett, int len);

enigma_score_function_t enigma_score_simple = { triscore_simple, biscore_simple, icscore_simple, uniscore_simple};

double icscore_simple(const decode_mapping_t* stbrett, int len)
{
  int f[26] = {0};
  double S = 0;
  int i;
  int c;

  if (len < 2)
    return 0;

  for (i = 0; i < len; i++) {
    c = decode(0,i,stbrett);
    S += unidict[c];
    f[c]++;
  }

  for (i = 0; i < 26; i++)
    S += f[i]*(f[i]-1);
  S /= len*(len-1);

  return S;
}

int uniscore_simple(const decode_mapping_t* stbrett, int len)
{
  int i;
  int c;
  int s = 0;

  for (i = 0; i < len; i++) {
    c = decode(0,i,stbrett);
    s += unidict[c];
  }

  return s;
}

int biscore_simple(const decode_mapping_t* stbrett, int len)
{
  int i;
  int c1, c2;
  int s = 0;

  c1 = decode(0,0,stbrett);

  for (i = 1; i < len; i++) {
    c2 = decode(0,i,stbrett);
    s += bidict[c1][c2];

    c1 = c2;
  }

  return s;

}

int triscore_simple(const decode_mapping_t* stbrett,  int len)
{
  int i;
  int c1, c2, c3;
  int s = 0;

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

