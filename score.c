#include <stdbool.h>
#include <string.h>
#include "cpu.h"
#include "cipher.h"
#include "dict.h"
#include "key.h"
#include "score.h"
#include "ciphertext.h"
#include "config\array_sizes.h"
#include "config\types.h"

#include "scoreBasic.h"
#include "scoreSimple.h"
#include "x86\scoreSsse3.h"

#ifdef TESTING_SCORE
# include "ScoreTesting.h"
#endif

/* declaration of internal functions */
void enigma_score_function_copy(enigma_score_function_t* restrict to, const enigma_score_function_t* restrict from);

inline
void enigma_score_function_copy(enigma_score_function_t* restrict to, const enigma_score_function_t* restrict prototype)
{
    to->triscore = prototype->triscore;
    to->biscore  = prototype->biscore;
    to->icscore  = prototype->icscore;
    to->uniscore = prototype->uniscore;
}

void enigma_score_init(enigma_cpu_flags_t cpu, enigma_score_function_t* sf)
{
#ifdef ENIGMA_CPU_SPECIFIC
    cpu = ENIGMA_CPU_SPECIFIC;
#endif // ENIGMA_CPU_SPECIFIC

    enigma_score_function_copy( sf, &enigmaScoreBasic );
    if (cpu & enigma_cpu_ssse3)
    {
        enigma_score_function_copy( sf,&enigmaScoreSsse3 );
    }

#ifdef TESTING_SCORE
    enigma_score_function_t* test = enigma_score_testing_create( EnigmaSF_Optimized, EnigmaSF_SSSE3 );
    enigma_score_function_copy(sf,test);
#endif
}

void DecodeScoredMessagePartStandard(const Key* const restrict key, int len, union ScoringDecodedMessage* restrict output){
    int i = 0;
    for( ; i < len; ++i ){
        output->plain[i] = decode( 0, i, &key->stbrett );
    }
}

bool GetDifferences( union ScoringDecodedMessage* restrict reference, union ScoringDecodedMessage* restrict tested, char* restrict output, int len ){
    char* ref = (char*)reference->plain;
    char* tes = (char*)tested->plain;
    int   i   = 0;
    bool  differenceSpotted = false;
    for( ; i < len; ++i ){
        if( ref[i] != tes[i] ){
            output[i] = '^';
            differenceSpotted = true;
        }
    }
    return differenceSpotted;
}

/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */
