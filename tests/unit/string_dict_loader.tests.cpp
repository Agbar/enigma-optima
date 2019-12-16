#include <string>

#include <gtest/gtest.h>

#include "dicts/dict_builders.hpp"
#include "dicts/string_dict_loader.hpp"

namespace {

struct storage_mock final
: enigma::dict_builder {

    storage_mock( std::string accepted_key )
    : accepted( accepted_key )
    , accepted_value( 0 )
    , called_cnt( 0 ) {}

    bool set_dict_value( const char (&key)[4], int value ) override {
        called_cnt++;
        if( accepted == key ) {
            accepted_value = value;
            return true;
        }
        return false;
    }

    const std::string accepted;
    int accepted_value;
    uint called_cnt;
};

}

TEST( storage_mock, accepts )
{
    storage_mock storage{ "ABC" };
    bool s = storage.set_dict_value( "ABC", 23 );
    EXPECT_TRUE( s );
}

TEST( storage_mock, rejects )
{
    storage_mock storage{ "ABC" };
    bool s = storage.set_dict_value( "ABD", 23 );
    EXPECT_FALSE( s );
}

TEST( string_dict_loader, loads_single_value )
{
    const char tri_input[] = "ABC\t42";

    storage_mock storage{ "ABC" };
    enigma::string_dict_loader dl{ "%3s%d", storage, tri_input };

    bool success = dl.load();

    EXPECT_TRUE( success );
    EXPECT_EQ( storage.accepted_value, 42 );
    EXPECT_EQ( storage.called_cnt, 1 );
}

TEST( string_dict_loader, fails_to_load_when_storage_rejects_value )
{
    const char tri_input[] = "AAA\t666";

    storage_mock storage{ "ABC" };
    enigma::string_dict_loader dl{ "%3s%d", storage, tri_input };

    bool success = dl.load();

    EXPECT_FALSE( success );
    EXPECT_EQ( storage.called_cnt, 1 );
}

TEST( string_dict_loader, loads_two_values )
{
    const char tri_input[] = "ABC\t42\n"
                             "ABC\t23";

    storage_mock storage{ "ABC" };
    enigma::string_dict_loader dl{ "%3s%d", storage, tri_input };

    bool success = dl.load();

    EXPECT_TRUE( success );
    EXPECT_EQ( storage.accepted_value, 23 );
    EXPECT_EQ( storage.called_cnt, 2 );
}

TEST( string_dict_loader, stops_at_first_storage_error )
{
    const char tri_input[] = "ABC\t42\n"
                             "ABB\t23\n"
                             "ABC\t6";

    storage_mock storage{ "ABC" };
    enigma::string_dict_loader dl{ "%3s%d", storage, tri_input };

    bool success = dl.load();

    EXPECT_FALSE( success );
    EXPECT_EQ( storage.called_cnt, 2 );
    EXPECT_EQ( storage.accepted_value, 42 );
}
