#ifndef INPUT_H
#define INPUT_H

#include "key.h"

PURE_FUNCTION
enum ModelType_t get_model(char *s);
int set_ukw(Key *key, char *s, enum ModelType_t model);
int set_walze(Key *key, char *s, enum ModelType_t model);
int set_ring(Key *key, char *s, enum ModelType_t model);
int set_mesg(Key *key, char *s, enum ModelType_t model);
int set_stecker(Key *key, char *s);
PURE_FUNCTION
int get_sw_mode(char *s);
PURE_FUNCTION
int get_firstpass(char *s);
int set_key(Key *key, const char *keystring, enum ModelType_t model, int adjust);
int set_range(Key *from, Key *to, const char *kf, const char *kt, enum ModelType_t model);

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
