extern "C" {
#include "dict.h"
#include "error.h"
}

#include "dicts/dict_builders.hpp"
#include "dicts/file_dict_loader.hpp"


int load_tridict(const char *filename)
{
    enigma::tri_dict_builder storage{};
    enigma::file_dict_loader tri{ "%3s%d", storage, filename };
    if( tri.load() ){
        return 0;
    }
    err_illegal_char_fatal( filename );
}

int load_bidict(const char *filename)
{
    enigma::bi_dict_builder storage{};
    enigma::file_dict_loader bi{ "%2s%d", storage, filename };
    if( bi.load() ){
        return 0;
    }
    err_illegal_char_fatal( filename );
}


int load_unidict(const char *filename)
{
    enigma::uni_dict_builder storage{};
    enigma::file_dict_loader uni{ "%1s%d", storage, filename };
    if( uni.load() ){
        return 0;
    }
    err_illegal_char_fatal( filename );
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */
