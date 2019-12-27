#include "dicts/dict_builders.hpp"
#include "dicts/string_dict_loader.hpp"

#include "str_dict.hpp"


namespace enigma {


bool strload_tridict( const char text[] ) {
    enigma::tri_dict_builder storage{};
    enigma::string_dict_loader tri{ "%3s%d", storage, text };
    return tri.load();
}


bool strload_bidict( const char text[] ) {
    enigma::bi_dict_builder storage{};
    enigma::string_dict_loader bi{ "%2s%d", storage, text };
    return bi.load();
}


bool strload_unidict( const char text[] ) {
    enigma::uni_dict_builder storage{};
    enigma::string_dict_loader uni{ "%1s%d", storage, text };
    return uni.load();
}


}