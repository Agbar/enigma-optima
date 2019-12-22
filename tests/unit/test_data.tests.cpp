#include <gtest/gtest.h>

#include "dicts/string_dict_loader.hpp"

extern "C" {
#include "enigma/test_data.h"
}

struct count_calls_storage_mock final
: enigma::dict_builder {

     count_calls_storage_mock()
    : called_cnt( 0 ) {}

    bool set_dict_value( const char (&key)[4], int value ) override {
        called_cnt++;
        return true;
    }

    uint called_cnt;
};

TEST( test_data, bigraph_cur_facts )
{
    count_calls_storage_mock storage{};
    enigma::string_dict_loader dl{ "%2s%d", storage, bigraph_cur };

    bool success = dl.load();
    EXPECT_TRUE( success );
    EXPECT_EQ( storage.called_cnt, 568 );
    EXPECT_EQ( bigraph_cur_size, 5113 );
}


TEST( test_data, unigraph_AVv1_facts )
{
    count_calls_storage_mock storage{};
    enigma::string_dict_loader dl{ "%1s%d", storage, unigraph_AVv1 };

    bool success = dl.load();
    EXPECT_TRUE( success );
    EXPECT_EQ( storage.called_cnt, 26 );
    EXPECT_EQ( unigraph_AVv1_size, 235 );
}
