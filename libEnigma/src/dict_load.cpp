#include <cstdio>
#include <cstdbool>
#include <cmath>

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
public:
    dict_loader( const char (&line_format)[6], dict_builder& storage_strategy )
    : lf( line_format )
    , storage( storage_strategy ) {}

    void load( const char *filename );

private:
    bool read_line( FILE* in_file ) {
        return fscanf( in_file, lf , key, &value ) != EOF;
    }

    char key[4];
    int value;

private:
    const char (&lf)[6];
    dict_builder& storage;
};

void dict_loader::load( const char *filename ) {
    FILE* fp = fopen( filename, "r" );
    if ( !fp ) err_open_fatal( filename );

    while ( read_line( fp ) ) {
        storage.set_dict_value( key, value, filename );
    }
    if ( fp ) fclose( fp );
}


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
    dict_loader tri{ "%3s%d", storage };
    tri.load( filename );
    return 0;
}

int load_bidict(const char *filename)
{
    bi_dict_builder storage{};
    dict_loader bi{ "%2s%d", storage };
    bi.load( filename );
    return 0;
}


int load_unidict(const char *filename)
{
    uni_dict_builder storage{};
    dict_loader uni{ "%1s%d", storage };
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
