#include <cstdio>
#include <cstdbool>
#include <cmath>

extern "C" {
#include "charmap.h"
#include "dict.h"
#include "echar/echar.h"
#include "error.h"
}

struct dict_loader {
    bool read_line( FILE* in_file ) {
        return fscanf( in_file, line_format() , chars, &log ) != EOF;
    }

    virtual void store_dict_value( const char* filename ) = 0;
    virtual const char* line_format() const = 0;

    char chars[4];
    int log;
};


struct tri_dict_loader
: dict_loader
{
    void store_dict_value( const char* filename ) override {
        if ( !echar_can_make_from_ascii( chars[0] )
          || !echar_can_make_from_ascii( chars[1] )
          || !echar_can_make_from_ascii( chars[2] ))
        {
            err_illegal_char_fatal( filename );
        }
        struct echar
            e0 = make_echar_ascii( chars[0] ),
            e1 = make_echar_ascii( chars[1] ),
            e2 = make_echar_ascii( chars[2] );
        tridict[echar_0_based_index( e0 )]
               [echar_0_based_index( e1 )]
               [echar_0_based_index( e2 )] = log;
    }

    const char* line_format() const final { return "%3s%d"; }
};

struct bi_dict_loader
: dict_loader
{
    void store_dict_value( const char* filename ) override {
        if ( !echar_can_make_from_ascii( chars[0] )
          || !echar_can_make_from_ascii( chars[1] ))
        {
            err_illegal_char_fatal( filename );
        }
        struct echar
            e0 = make_echar_ascii( chars[0] ),
            e1 = make_echar_ascii( chars[1] );
        bidict[echar_0_based_index( e0 )]
              [echar_0_based_index( e1 )] = log;
    }

    const char* line_format() const final { return "%2s%d"; }
};

struct uni_dict_loader
: dict_loader
{
    void store_dict_value( const char* filename ) override {
        if ( !echar_can_make_from_ascii( chars[0] ))
        {
            err_illegal_char_fatal( filename );
        }
        struct echar e = make_echar_ascii( chars[0] );
        unidict[echar_0_based_index( e )] = log;
    }

    const char* line_format() const final { return "%1s%d"; }
};


static void load_anydict( const char *filename, struct dict_loader* impl );


void load_anydict( const char *filename, struct dict_loader* impl ) {
    FILE* fp = fopen( filename, "r" );
    if ( !fp ) err_open_fatal( filename );

    while ( impl->read_line( fp ) ) {
        impl->store_dict_value( filename );
    }
    if ( fp ) fclose( fp );
}

int load_tridict(const char *filename)
{
    tri_dict_loader tri{};
    load_anydict( filename, &tri );
    return 0;
}

int load_bidict(const char *filename)
{
    bi_dict_loader bi{};
    load_anydict( filename, &bi );
    return 0;
}


int load_unidict(const char *filename)
{
    uni_dict_loader uni{};
    load_anydict( filename, &uni );
    return 0;
}


/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */
