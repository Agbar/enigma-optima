#pragma once
/*
 * This file is part of Enigma Optima, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */

#include <cstdio>
#include <memory>

#include "dicts/dict_loader.hpp"

namespace enigma {


class string_dict_loader
: public enigma::dict_loader
{
public:
    string_dict_loader(
        const char (&line_format)[6],
        enigma::dict_builder& storage_strategy,
        const char* input_buffer )
    : dict_loader( line_format, storage_strategy )
    , buffer( input_buffer )
    , ext_line_format( std::string( line_format ) + "%n" )
    {}

    string_dict_loader(const string_dict_loader&) = delete;
    string_dict_loader& operator= (const string_dict_loader&) = delete;
protected:
    bool read_line() override {
        int scanned_cnt;
        int r = std::sscanf( buffer, ext_line_format.c_str() , key, &value, &scanned_cnt );
        if( r == EOF ) return false;
        else {
            buffer += scanned_cnt;
            return true;
        }
    }

private:
    const char* buffer;
    std::string ext_line_format;
};


}
