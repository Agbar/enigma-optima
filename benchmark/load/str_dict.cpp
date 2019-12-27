#include "dicts/dict_builders.hpp"
#include "dicts/string_dict_loader.hpp"

#include "str_dict.hpp"

extern "C" {
#include "dict.h"
}

namespace enigma {
namespace {

    template< class dict_type, dict_type &dict >
    class dict_reset_policy final {
        const char* last_source = nullptr;

    public:
        dict_reset_policy() = default;

        dict_reset_policy( dict_reset_policy& ) = delete;
        dict_reset_policy& operator=( dict_reset_policy& ) = delete;

        __attribute__((warn_unused_result))
        bool check_source_changed( const char source[] ) {
            bool chg = last_source != source;
            if ( last_source && chg ) {
                reset_dict( dict );
            }
            last_source = source;
            return chg;
        }

        template< size_t L, size_t M, size_t N >
        void reset_dict( dict_t (&d)[L][M][N] ) {
            for( auto& i: d ){
                reset_dict( i );
            }
        }

        template< size_t M, size_t N >
        void reset_dict( dict_t (&d)[M][N] ) {
            for( auto& i: d ){
                reset_dict( i );
            }
        }


        template< size_t N >
        void reset_dict( dict_t (&d)[N] ) {
            for( auto& i: d ){
                i = 0;
            }
        }
    };
}

bool strload_tridict( const char text[] ) {
    static dict_reset_policy< decltype(tridict), tridict > drp{};
    if( drp.check_source_changed( text ) ){
        enigma::tri_dict_builder storage{};
        enigma::string_dict_loader tri{ "%3s%d", storage, text };
        return tri.load();
    }
    return true;
}


bool strload_bidict( const char text[] ) {
    static dict_reset_policy< decltype(bidict), bidict > drp{};
    if( drp.check_source_changed( text ) ) {
        enigma::bi_dict_builder storage{};
        enigma::string_dict_loader bi{ "%2s%d", storage, text };
        return bi.load();
    }
    return true;
}


bool strload_unidict( const char text[] ) {
    static dict_reset_policy< decltype(unidict), unidict > drp{};
    if( drp.check_source_changed( text ) ) {
        enigma::uni_dict_builder storage{};
        enigma::string_dict_loader uni{ "%1s%d", storage, text };
        return uni.load();
    }
    return true;
}


}