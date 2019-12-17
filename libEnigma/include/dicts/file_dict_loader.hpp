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

extern "C" {
#include "error.h"
}


namespace enigma {


class file_dict_loader
: public enigma::dict_loader
{
public:
    file_dict_loader(
        const char (&line_format)[6],
        enigma::dict_builder& storage_strategy,
        const char* filename )
    : dict_loader( line_format, storage_strategy )
    , file( std::fopen( filename, "r" ), &close_file )
    , file_name( filename )
    {
        if ( !file ) err_open_fatal( filename );
    }

    file_dict_loader( file_dict_loader& ) = delete;
    file_dict_loader& operator= ( file_dict_loader& ) = delete;

protected:
    bool read_line() override {
        return std::fscanf( file.get(), lf , key, &value ) != EOF;
    }

private:
    static void close_file( FILE* f ) {
        if ( f ) std::fclose ( f );
    }
    std::unique_ptr<FILE, decltype(&close_file)> file;
    const char* file_name;
};


}