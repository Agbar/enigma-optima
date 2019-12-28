#pragma once

#include <stdbool.h>

#include "common.h"
#include "key.h"

PURE_FUNCTION
enum ModelType_t get_model(char *s);
int set_ukw( struct Key *key, char *s, enum ModelType_t model);
int set_walze( struct Key *key, char *s, enum ModelType_t model);
bool set_RingsState( struct RingsState* rs, const char* s, enum ModelType_t model );
int set_ring( struct Key *key, const char *s, enum ModelType_t model );
int set_mesg( struct Key *key, char *s, enum ModelType_t model);
int set_stecker( struct Key *key, char *s);
PURE_FUNCTION
int get_sw_mode(char *s);
PURE_FUNCTION
int get_firstpass(char *s);
int set_key( struct Key *key, const char *keystring, enum ModelType_t model, int adjust);
int set_range( struct Key *restrict from, struct Key *restrict to, const char *kf, const char *kt, enum ModelType_t model );


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
