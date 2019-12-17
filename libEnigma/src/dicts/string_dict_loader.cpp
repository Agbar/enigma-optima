/*
 * This file is part of Enigma Optima, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */

#include <cstdio>

#include "dicts/string_dict_loader.hpp"

namespace enigma {

string_dict_loader::string_dict_loader(
    const char (&line_format)[6],
    enigma::dict_builder& storage_strategy,
    const char* input_buffer )
: dict_loader( line_format, storage_strategy )
, buffer( input_buffer )
, ext_line_format( std::string( line_format ) + "%n" )
{}


bool string_dict_loader::read_line() {
    int scanned_cnt;
    int r = std::sscanf( buffer, ext_line_format.c_str() , key, &value, &scanned_cnt );
    if( r == EOF ) return false;
    else {
        buffer += scanned_cnt;
        return true;
    }
}

}
