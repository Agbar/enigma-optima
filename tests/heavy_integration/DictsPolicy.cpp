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

static bool load_tridict( const char text[] ) {
    enigma::tri_dict_builder storage{};
    enigma::string_dict_loader tri{"%3s%d", storage, text};
    return tri.load();
}


static bool load_bidict( const char text[] ) {
    enigma::bi_dict_builder storage{};
    enigma::string_dict_loader bi{"%2s%d", storage, text};
    return bi.load();
}

} // namespace heavy_test


void Krah1941DictsPolicy::LoadDicts() {

    ASSERT_TRUE( heavy_test::load_tridict( trigraph_cur ) );
    ASSERT_TRUE( heavy_test::load_bidict( bigraph_cur ) );
}


void Krah1941DictsPolicy::ClearDicts() {
    std::memset( tridict, 0, sizeof tridict );
    std::memset( bidict, 0, sizeof bidict );
}
