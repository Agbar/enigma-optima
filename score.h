#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

#include "cpu.h"
#include "key.h"

typedef struct _enigma_score_function_t{
    int    (*triscore) (const Key* const restrict key, int len);
    int    (* biscore) (const Key* const restrict key, int len);
    double (* icscore) (const Key* const restrict key, int len);
    int    (*uniscore) (const Key* const restrict key, int len);
} enigma_score_function_t;

// Initializes sf based on current cpu features.
/** \brief
 *
 * \param cpu int
 * \param sf enigma_score_function_t*
 * \return void
 *
 */
void enigma_score_init(enigma_cpu_flags_t cpu, enigma_score_function_t* restrict sf);


/** \brief Uses original code. Used as the best tested reference.
  */
extern enigma_score_function_t enigma_score_orig;


/** \brief Uses optimized version of original code. Gives minor performance improvement.
 */
extern enigma_score_function_t enigma_score_opt;

int get_triscore(const Key *key, int len);

#endif


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
