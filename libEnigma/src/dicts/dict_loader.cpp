/*
 * This file is part of Enigma Optima, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */

#include "dicts/dict_loader.hpp"

namespace enigma {


bool dict_loader::load() {
    while ( read_line() ) {
        if( !storage.set_dict_value( key, value ) ){
            return false;
        }
    }
    return true;
}


}