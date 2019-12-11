#pragma once
/*
 * This file is part of Enigma Optima, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 * Copyright (C) 2019 Agbar
 */

namespace enigma {

struct dict_builder {
    virtual bool set_dict_value( char (&key)[4], int value ) = 0;
};


struct tri_dict_builder
: dict_builder
{
    bool set_dict_value( char (&key)[4], int value ) override;
};


struct bi_dict_builder
: dict_builder
{
    bool set_dict_value( char (&key)[4], int value ) override;
};


struct uni_dict_builder
: dict_builder
{
    bool set_dict_value( char (&key)[4], int value ) override;
};


}