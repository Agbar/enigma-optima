#ifndef KEY_H
#define KEY_H

#include <stdint.h>

#include "global.h"
#include "config\types.h"

/* PermutationMap
 * * * * * * * * */
typedef union _PermutationMap_t
{
    v32qi whole;

    v16qi half[2];

    text_t letters[32];
} PermutationMap_t;

inline
void FixPermutationMapTail(PermutationMap_t* mapping){
    int k = 26;
    for(; k < 32; k++)
    {
        mapping->letters[k] = mapping->letters[k-26];
    }
}

/* RingsState
 * * * * * * */
struct RingsState
{
    int8_t g, l, m, r;
};

/* Model
 * * * * */
 enum ModelType_t{
    EnigmaModel_Error   = -1,
    EnigmaModel_H       =  0,
    EnigmaModel_M3      =  1,
    EnigmaModel_M4      =  2,
 };

/* Key
 * * * */
typedef struct _key_t {
    ALIGNED_16(PermutationMap_t stbrett);
    struct RingsState slot; ///< Contains numbers of rings in slots. /* greek, left, middle, right slot */
    struct RingsState ring; ///< ringstellungen
    struct RingsState mesg; ///< message settings
    int8_t ukwnum;
    enum ModelType_t model;
    text_t sf[26];     /* swapped/free letters */
    int count;      /* number of swapped letters */
    int score;      /* hillclimbing score */
} Key;

int init_key_default(Key *key, enum ModelType_t model);
int init_key_low(Key *key, enum ModelType_t model);
int keycmp(const Key *k1, const Key *k2);

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
