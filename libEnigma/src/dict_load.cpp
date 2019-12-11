#include <cstdio>
#include <cstdbool>
#include <cmath>
#include <memory>

extern "C" {
#include "dict.h"
#include "error.h"
}

#include "dicts/dict_builders.hpp"

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

bool dict_loader::load() {
    while ( read_line() ) {
        if( !storage.set_dict_value( key, value ) ){
            return false;
        }
    }
    return true;
}


class file_dict_loader
: public dict_loader
{
public:
    file_dict_loader( const char (&line_format)[6], enigma::dict_builder& storage_strategy, const char* filename )
    : dict_loader( line_format, storage_strategy )
    , file( fopen( filename, "r" ), &close_file )
    , file_name( filename )
    {
        if ( !file ) err_open_fatal( filename );
    }

protected:
    bool read_line() override {
        return fscanf( file.get(), lf , key, &value ) != EOF;
    }


private:
    static void close_file( FILE* f ) {
        if ( f ) fclose ( f );
    }
    std::unique_ptr<FILE, decltype(&close_file)> file;
    const char* file_name;
};



int load_tridict(const char *filename)
{
    enigma::tri_dict_builder storage{};
    file_dict_loader tri{ "%3s%d", storage, filename };
    if( tri.load() ){
        return 0;
    }
    err_illegal_char_fatal( filename );
}

int load_bidict(const char *filename)
{
    enigma::bi_dict_builder storage{};
    file_dict_loader bi{ "%2s%d", storage, filename };
    if( bi.load() ){
        return 0;
    }
    err_illegal_char_fatal( filename );
}


int load_unidict(const char *filename)
{
    enigma::uni_dict_builder storage{};
    file_dict_loader uni{ "%1s%d", storage, filename };
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
