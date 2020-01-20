#include "DictsPolicy.hpp"

#include <cstring>
#include <gtest/gtest.h>


#include "dicts/dict_builders.hpp"
#include "dicts/string_dict_loader.hpp"


extern "C" {
#include "dict.h"
#include "enigma/test_data.h"
}


namespace heavy_test {

bool load_tridict( const char text[] ) {
    enigma::tri_dict_builder storage{};
    enigma::string_dict_loader tri{"%3s%d", storage, text};
    return tri.load();
}


bool load_bidict( const char text[] ) {
    enigma::bi_dict_builder storage{};
    enigma::string_dict_loader bi{"%2s%d", storage, text};
    return bi.load();
}


bool load_unidict( const char text[] ) {
    enigma::uni_dict_builder storage{};
    enigma::string_dict_loader uni{"%1s%d", storage, text};
    return uni.load();
}


void clear_tridict() {
    std::memset( tridict, 0, sizeof tridict );
}


void clear_bidict() {
    std::memset( bidict, 0, sizeof bidict );
}


void clear_unidict() {
    std::memset( unidict, 0, sizeof unidict );
}


} // namespace heavy_test
