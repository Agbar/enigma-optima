#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "charmap.h"
#include "dict.h"
#include "echar/echar.h"
#include "error.h"

struct dict_loader;

struct dict_loader_vt {
    bool (*read_line)( struct dict_loader* self, FILE* in_file );
    void (*store_dict_value)( struct dict_loader* self, const char* filename );
};

struct dict_loader {
    struct dict_loader_vt* vt;
    char chars[4];
    int log;
};

static void load_anydict( const char *filename, struct dict_loader* impl );

static bool tri_read_line( struct dict_loader* self, FILE* in_file );
static void tri_store_dict_value( struct dict_loader* self, const char* filename );

static bool bi_read_line( struct dict_loader* self, FILE* in_file );
static void bi_store_dict_value( struct dict_loader* self, const char* filename );

static bool uni_read_line( struct dict_loader* self, FILE* in_file );
static void uni_store_dict_value( struct dict_loader* self, const char* filename );

struct dict_loader_vt
    tri_load_cls = { tri_read_line, tri_store_dict_value },
     bi_load_cls = {  bi_read_line,  bi_store_dict_value },
    uni_load_cls = { uni_read_line, uni_store_dict_value };


bool tri_read_line( struct dict_loader* self, FILE* in_file ) {
    return fscanf( in_file, "%3s%d", self->chars, &self->log ) != EOF;
}

bool bi_read_line( struct dict_loader* self, FILE* in_file ) {
    return fscanf( in_file, "%2s%d", self->chars, &self->log ) != EOF;
}

bool uni_read_line( struct dict_loader* self, FILE* in_file ) {
    return fscanf( in_file, "%1s%d", self->chars, &self->log ) != EOF;
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

    while ( impl->vt->read_line( impl, fp ) ) {
        impl->vt->store_dict_value( impl, filename );
    }
    if ( fp ) fclose( fp );
}

int load_tridict(const char *filename)
{
    struct dict_loader tri = { .vt = &tri_load_cls };
    load_anydict( filename, &tri );
    return 0;
}

int load_bidict(const char *filename)
{
    struct dict_loader bi = { .vt = &bi_load_cls };
    load_anydict( filename, &bi );
    return 0;
}


int load_unidict(const char *filename)
{
    struct dict_loader uni = { .vt = &uni_load_cls };
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
