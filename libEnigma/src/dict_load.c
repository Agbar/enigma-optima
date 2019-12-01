#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "charmap.h"
#include "dict.h"
#include "echar/echar.h"
#include "error.h"

struct dict_loader;

struct dict_loader_vt {
    void (* const store_dict_value)( struct dict_loader* self, const char* filename );
    char* const line_format;
};

struct dict_loader {
    const struct dict_loader_vt* const vt;
    char chars[4];
    int log;
};

// 'constructors':
struct dict_loader make_tri_dict_loader();
struct dict_loader make_bi_dict_loader();
struct dict_loader make_uni_dict_loader();

static void load_anydict( const char *filename, struct dict_loader* impl );

static bool any_read_line( struct dict_loader* self, FILE* in_file );

// 'virtuals':
static void tri_store_dict_value( struct dict_loader* self, const char* filename );
static void bi_store_dict_value( struct dict_loader* self, const char* filename );
static void uni_store_dict_value( struct dict_loader* self, const char* filename );

static char tri_line_format[] = "%3s%d";
static char  bi_line_format[] = "%2s%d";
static char uni_line_format[] = "%1s%d";


const struct dict_loader_vt
    tri_load_cls = { tri_store_dict_value, tri_line_format },
     bi_load_cls = {  bi_store_dict_value,  bi_line_format },
    uni_load_cls = { uni_store_dict_value, uni_line_format };

struct dict_loader make_tri_dict_loader() {
    return (struct dict_loader) { .vt = &tri_load_cls };
}

struct dict_loader make_bi_dict_loader() {
    return (struct dict_loader) { .vt = &bi_load_cls };
}

struct dict_loader make_uni_dict_loader() {
    return (struct dict_loader) { .vt = &uni_load_cls };
}


bool any_read_line( struct dict_loader* self, FILE* in_file ) {
    return fscanf( in_file, self->vt->line_format , self->chars, &self->log ) != EOF;
}


void tri_store_dict_value( struct dict_loader* self, const char* filename ) {
    if ( !echar_can_make_from_ascii( self->chars[0] )
      || !echar_can_make_from_ascii( self->chars[1] )
      || !echar_can_make_from_ascii( self->chars[2] ))
    {
        err_illegal_char_fatal( filename );
    }
    struct echar
        e0 = make_echar_ascii( self->chars[0] ),
        e1 = make_echar_ascii( self->chars[1] ),
        e2 = make_echar_ascii( self->chars[2] );
    tridict[echar_0_based_index( e0 )]
           [echar_0_based_index( e1 )]
           [echar_0_based_index( e2 )] = self->log;
}

void bi_store_dict_value( struct dict_loader* self, const char* filename ) {
    if ( !echar_can_make_from_ascii( self->chars[0] )
      || !echar_can_make_from_ascii( self->chars[1] ))
    {
        err_illegal_char_fatal( filename );
    }
    struct echar
        e0 = make_echar_ascii( self->chars[0] ),
        e1 = make_echar_ascii( self->chars[1] );
    bidict[echar_0_based_index( e0 )]
          [echar_0_based_index( e1 )] = self->log;
}

void uni_store_dict_value( struct dict_loader* self, const char* filename ) {
    if ( !echar_can_make_from_ascii( self->chars[0] ))
    {
        err_illegal_char_fatal( filename );
    }
    struct echar e = make_echar_ascii( self->chars[0] );
    unidict[echar_0_based_index( e )] = self->log;
}

void load_anydict( const char *filename, struct dict_loader* impl ) {
    FILE* fp = fopen( filename, "r" );
    if ( !fp ) err_open_fatal( filename );

    while ( any_read_line( impl, fp ) ) {
        impl->vt->store_dict_value( impl, filename );
    }
    if ( fp ) fclose( fp );
}

int load_tridict(const char *filename)
{    
    struct dict_loader tri = make_tri_dict_loader();
    load_anydict( filename, &tri );
    return 0;
}

int load_bidict(const char *filename)
{
    struct dict_loader bi = make_bi_dict_loader();
    load_anydict( filename, &bi );
    return 0;
}


int load_unidict(const char *filename)
{
    struct dict_loader uni = make_uni_dict_loader();
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
