#include <cstdio>
#include <cstdbool>
#include <cmath>
#include <memory>

extern "C" {
#include "charmap.h"
#include "dict.h"
#include "echar/echar.h"
#include "error.h"
}

struct dict_builder {
    virtual void set_dict_value( char (&key)[4], int value, const char* filename ) = 0;
};

class dict_loader {
protected:
    dict_loader( const char (&line_format)[6], dict_builder& storage_strategy )
    : lf( line_format )
    , storage( storage_strategy ) {}

public:
    void load( const char *filename );

protected:
    virtual bool read_line() = 0;
    
    char key[4];
    int value;

    const char (&lf)[6];
private:
    dict_builder& storage;
};

void dict_loader::load( const char *filename ) {
    while ( read_line() ) {
        storage.set_dict_value( key, value, filename );
    }
}


class file_dict_loader
: public dict_loader
{
public:
    file_dict_loader( const char (&line_format)[6], dict_builder& storage_strategy, const char* filename )
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


struct tri_dict_builder
: dict_builder
{
    void set_dict_value( char (&key)[4], int value, const char* filename ) override {
        if ( !echar_can_make_from_ascii( key[0] )
          || !echar_can_make_from_ascii( key[1] )
          || !echar_can_make_from_ascii( key[2] ))
        {
            err_illegal_char_fatal( filename );
        }
        struct echar
            e0 = make_echar_ascii( key[0] ),
            e1 = make_echar_ascii( key[1] ),
            e2 = make_echar_ascii( key[2] );
        tridict[echar_0_based_index( e0 )]
               [echar_0_based_index( e1 )]
               [echar_0_based_index( e2 )] = value;
    }
};


struct bi_dict_builder
: dict_builder
{
    void set_dict_value( char (&key)[4], int value, const char* filename ) override {
        if ( !echar_can_make_from_ascii( key[0] )
          || !echar_can_make_from_ascii( key[1] ))
        {
            err_illegal_char_fatal( filename );
        }
        struct echar
            e0 = make_echar_ascii( key[0] ),
            e1 = make_echar_ascii( key[1] );
        bidict[echar_0_based_index( e0 )]
              [echar_0_based_index( e1 )] = value;
    }
};


struct uni_dict_builder
: dict_builder
{
    void set_dict_value( char (&key)[4], int value, const char* filename ) override {
        if ( !echar_can_make_from_ascii( key[0] ))
        {
            err_illegal_char_fatal( filename );
        }
        struct echar e = make_echar_ascii( key[0] );
        unidict[echar_0_based_index( e )] = value;
    }
};


int load_tridict(const char *filename)
{
    tri_dict_builder storage{};
    file_dict_loader tri{ "%3s%d", storage, filename };
    tri.load( filename );
    return 0;
}

int load_bidict(const char *filename)
{
    bi_dict_builder storage{};
    file_dict_loader bi{ "%2s%d", storage, filename };
    bi.load( filename );
    return 0;
}


int load_unidict(const char *filename)
{
    uni_dict_builder storage{};
    file_dict_loader uni{ "%1s%d", storage, filename };
    uni.load( filename );
    return 0;
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */
