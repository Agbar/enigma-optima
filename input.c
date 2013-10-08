#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "charmap.h"
#include "error.h"
#include "global.h"
#include "key.h"
#include "stecker.h"
#include "input.h"

/** \brief Determine model.
 */
enum ModelType_t get_model(char *s)
{
  if (strcmp(s, "H") == 0 || strcmp(s, "h") == 0)
    return EnigmaModel_H;
  if (strcmp(s, "M3") == 0 || strcmp(s, "m3") == 0)
    return EnigmaModel_M3;
  if (strcmp(s, "M4") == 0 || strcmp(s, "m4") == 0)
    return EnigmaModel_M4;

  return EnigmaModel_Error;
}

/* set UKW */
int set_ukw(Key *key, char *s, enum ModelType_t model)
{
  if (strcmp(s, "A") == 0 || strcmp(s, "a") == 0) {
    switch (model) {
      case EnigmaModel_H: key->ukwnum = 0; break;
      default: return 0;
    }
    return 1;
  }
  if (strcmp(s, "B") == 0 || strcmp(s, "b") == 0) {
    switch (model) {
      case EnigmaModel_M4: key->ukwnum = 3; break;
      default: key->ukwnum = 1; break;
    }
    return 1;
  }
  if (strcmp(s, "C") == 0 || strcmp(s, "c") == 0) {
    switch (model) {
      case EnigmaModel_M4: key->ukwnum = 4; break;
      default: key->ukwnum = 2; break;
    }
    return 1;
  }

  return 0;
}

/** \brief Set walzen.
 *
 * \param key Key*
 * \param s char*
 * \param model enum ModelType_t
 * \return int
 *
 */
int set_walze(Key *key, char *s, enum ModelType_t model)
{
  char *x;

  switch (model) {
    case EnigmaModel_M4: if (strlen(s) != 4) return 0; break;
    default: if (strlen(s) != 3) return 0; break;
  }

  x = s;
  if (model == EnigmaModel_M4) { /* greek wheel */
    if ( !(*x == 'B' || *x == 'b' || *x == 'G' || *x == 'g') )
      return 0;
    x++;
  }
  while (*x != '\0') {
    switch (model) {
      case EnigmaModel_H: /* digits 1-5, no repetitions */
        if ( !isdigit((unsigned char)*x) || *x < '0' || *x > '5'
           || strrchr(x, *x) != x )
             return 0;
        break;
      case EnigmaModel_M3: case EnigmaModel_M4: /* digits 1-8, no repetitions */
        if ( !isdigit((unsigned char)*x) || *x < '0' || *x > '8'
           || strrchr(x, *x) != x )
             return 0;
        break;
      default:
        return 0;
    }
    x++;
  }

  x = s;
  if (model == EnigmaModel_M4) {
    if (*x == 'B' || *x == 'b')
      key->slot.g = 9;
    if (*x == 'G' || *x == 'g')
      key->slot.g = 10;
    x++;
  }
  key->slot.l = *x++ - '0';
  key->slot.m = *x++ - '0';
  key->slot.r = *x - '0';

  return 1;
}

/** \brief Set rings.
 *
 * \param key Key*
 * \param s char*
 * \param model enum ModelType_t
 * \return int
 *
 */
int set_ring(Key *key, char *s, enum ModelType_t model)
{
  char *x;

  switch (model) {
    case EnigmaModel_M4: if (strlen(s) != 4) return 0; break;
    default: if (strlen(s) != 3) return 0; break;
  }

  x = s;
  while (*x != '\0') {
    if (code[(unsigned char)*x] == 26)
      return 0;
    x++;
  }

  x = s;
  if (model == EnigmaModel_M4)
    key->ring.g = code[(unsigned char)*x++];
  key->ring.l = code[(unsigned char)*x++];
  key->ring.m = code[(unsigned char)*x++];
  key->ring.r = code[(unsigned char)*x];

  return 1;
}

/** \brief Set message keys.
 *
 * \param key Key*
 * \param s char*
 * \param model enum ModelType_t
 * \return int
 *
 */
int set_mesg(Key *key, char *s, enum ModelType_t model)
{
  char *x;

  switch (model) {
    case EnigmaModel_M4: if (strlen(s) != 4) return 0; break;
    default: if (strlen(s) != 3) return 0; break;
  }

  x = s;
  while (*x != '\0') {
    if (code[(unsigned char)*x] == 26)
      return 0;
    x++;
  }

  x = s;
  if (model == EnigmaModel_M4)
    key->mesg.g = code[(unsigned char)*x++];
  key->mesg.l = code[(unsigned char)*x++];
  key->mesg.m = code[(unsigned char)*x++];
  key->mesg.r = code[(unsigned char)*x];

  return 1;
}

/** \brief Set steckerbrett.
 *
 * \param key Key*
 * \param s char*
 * \return int
 *
 */
int set_stecker(Key *key, char *s)
{
  int len;
  char *x;

  /* max 26 chars, even number */
  if ((len = strlen(s)) > 26 || len%2 != 0)
        return 0;

  x = s;
  while (*x != '\0') {
    *x = tolower((unsigned char)*x);
    x++;
  }

  x = s;
  while (*x != '\0') {
    /* alphabetic, no repetitions */
    if (code[(unsigned char)*x] == 26 || strrchr(x, *x) != x)
      return 0;
    x++;
  }

  /* swap appropriate letters */
  x = s;
  while (*x != '\0') {
    SwapStbrett(key, code[(unsigned char)*x], code[(unsigned char)*(x+1)]);
    x += 2;
  }

  return 1;
}

/** \brief Determine mode for slow ring.
 *
 * \param s char*
 * \return int
 *
 */
int get_sw_mode(char *s)
{
  if (strcmp(s, "0") == 0)
    return SW_ONSTART;
  if (strcmp(s, "1") == 0)
    return SW_OTHER;
  if (strcmp(s, "2") == 0)
    return SW_ALL;

  return -1;
}

/** \brief Get firstpass.
 *
 * \param s char*
 * \return int
 *
 */
int get_firstpass(char *s)
{
  if (strcmp(s, "0") == 0)
    return 0;
  if (strcmp(s, "1") == 0)
    return 1;

  return -1;
}

/** \brief Set *key according to *keystring, model.
 *
 * \param key Key*
 * \param keystring const char*
 * \param model enum ModelType_t
 * \param adjust int
 * \return int
 *
 */
int set_key(Key *key, const char *keystring, enum ModelType_t model, int adjust)
{
    int i, d;
    unsigned int len;
    char s[15];
    char ring[5];
    char *x;


    if (!init_key_low(key, model)) return 0;

    switch (model) {
      case EnigmaModel_H: case EnigmaModel_M3: len = 12; d = 4; break;
      case EnigmaModel_M4: len = 14; d = 5; break;
      default: return 0;
    }

    if (strlen(keystring) != len) return 0;
    strcpy(s, keystring);

    i = 1;
    if (s[i] != ':') return 0;
    s[i] = '\0';
    i += d;
    if (s[i] != ':') return 0;
    s[i] = '\0';
    i += 3;
    if (s[i] != ':') return 0;
    s[i] = '\0';

    x = s;
    if (!set_ukw(key, x, model)) return 0;

    x += 2;
    if (!set_walze(key, x, model)) return 0;

    x += d;
    if (model == EnigmaModel_M4)
      sprintf(ring, "AA");
    else
      sprintf(ring, "A");
    strcat(ring, x);
    if (!set_ring(key, ring, model)) return 0;

    x += 3;
    if (!set_mesg(key, x, model)) return 0;


    /* error checking for rings */
    if ( key->slot.m > 5 && key->ring.m > 12 ) {
      if (adjust) {
        key->ring.m = (key->ring.m + 13) % 26;
        key->mesg.m = (key->mesg.m + 13) % 26;
      }
      else
        err_input_fatal(ERR_RING_SHORTCUT);
    }
    if ( key->slot.r > 5 && key->ring.r > 12 ) {
      if (adjust) {
        key->ring.r = (key->ring.r + 13) % 26;
        key->mesg.r = (key->mesg.r + 13) % 26;
      }
      else
        err_input_fatal(ERR_RING_SHORTCUT);
    }

    return 1;
}

/** \brief Set keys *from, *to according to [keystrings kf, kt], model.
 *
 * \param from Key*
 * \param to Key*
 * \param kf const char*
 * \param kt const char*
 * \param model enum ModelType_t
 * \return int
 *
 */
int set_range(Key *from, Key *to, const char *kf, const char *kt, enum ModelType_t model)
{
  if (!set_key(from, kf, model, 0)) return 0;
  if (!set_key(to, kt, model, 0)) return 0;
  if (keycmp(from, to) == 1) return 0;

  return 1;

}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
