#pragma once
/*
 * This file is part of Enigma Optima, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */

#include "dicts/dict_builders.hpp"

namespace enigma {

class dict_loader {
protected:
    dict_loader( const char (&line_format)[6], enigma::dict_builder& storage_strategy )
    : lf( line_format )
    , storage( storage_strategy ) {}

public:
    bool load();

protected:
    virtual bool read_line() = 0;
    
    char key[4];
    int value;

    const char (&lf)[6];
private:
    enigma::dict_builder& storage;
};


}