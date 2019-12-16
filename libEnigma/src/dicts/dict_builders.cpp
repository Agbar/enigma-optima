/*
 * This file is part of Enigma Optima, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */

#include "dicts/dict_builders.hpp"

extern "C" {
#include "dict.h"
#include "echar/echar.h"
}

namespace enigma{

bool tri_dict_builder:: set_dict_value( const char (&key)[4], int value ) {
    if ( !echar_can_make_from_ascii( key[0] )
      || !echar_can_make_from_ascii( key[1] )
      || !echar_can_make_from_ascii( key[2] ))
    {
        return false;
    }
    struct echar
        e0 = make_echar_ascii( key[0] ),
        e1 = make_echar_ascii( key[1] ),
        e2 = make_echar_ascii( key[2] );
    tridict[echar_0_based_index( e0 )]
           [echar_0_based_index( e1 )]
           [echar_0_based_index( e2 )] = value;
    return true;
}


bool bi_dict_builder::set_dict_value( const char (&key)[4], int value ) {
    if ( !echar_can_make_from_ascii( key[0] )
      || !echar_can_make_from_ascii( key[1] ))
    {
        return false;
    }
    struct echar
        e0 = make_echar_ascii( key[0] ),
        e1 = make_echar_ascii( key[1] );
    bidict[echar_0_based_index( e0 )]
          [echar_0_based_index( e1 )] = value;
    return true;
}

bool uni_dict_builder::set_dict_value( const char (&key)[4], int value ) {
    if ( !echar_can_make_from_ascii( key[0] ))
    {
        return false;
    }
    struct echar e = make_echar_ascii( key[0] );
    unidict[echar_0_based_index( e )] = value;
    return true;
}


}