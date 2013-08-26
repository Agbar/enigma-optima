#ifndef KEY_H
#define KEY_H

#include <stdint.h>

#include "global.h"
#include "config\types.h"

typedef union _DecodeMapping
{
    v32qi whole;

    v16qi half[2];

    text_t letters[32];
} decode_mapping_t;

typedef struct _key_t
{
        int model;
        int8_t ukwnum;
        /* greek, left, middle, right slot */
        struct
        {
            int8_t g_slot;
            int8_t l_slot;
            int8_t m_slot;
            int8_t r_slot;
        };
        /* ringstellungen */
        struct
        {
            text_t g_ring;
            text_t l_ring;
            text_t m_ring;
            text_t r_ring;
        };
        /* message settings */
        struct
        {
            text_t g_mesg;
            text_t l_mesg;
            text_t m_mesg;
            text_t r_mesg;
        };
        ALIGNED_16(decode_mapping_t stbrett);
        text_t sf[26];     /* swapped/free letters */
        int count;      /* number of swapped letters */
        int score;      /* hillclimbing score */
} Key;

int init_key_default(Key *key, int model);
int init_key_low(Key *key, int model);
int keycmp(const Key *k1, const Key *k2);

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
