#include <string>
#include <gtest/gtest.h>

#include "dicts/string_dict_loader.hpp"

extern "C" {
#include "enigma/test_data.h"
#include "global.h"
}

struct count_calls_storage_mock final
: enigma::dict_builder {

     count_calls_storage_mock()
    : called_cnt( 0 ) {}

    bool set_dict_value( UNUSED const char (&key)[4], UNUSED int value ) override {
        called_cnt++;
        return true;
    }

    unsigned int called_cnt;
};

TEST( test_data, benchmark_cipher_pbnxa_facts )
{
    EXPECT_EQ( std::string( benchmark_cipher_pbnxa, 5 ), "pbnxa" );
    EXPECT_EQ( benchmark_cipher_pbnxa_size, 187 );
}


TEST( test_data, bigraph_cur_facts )
{
    count_calls_storage_mock storage{};
    enigma::string_dict_loader dl{ "%2s%d", storage, bigraph_cur };

    bool success = dl.load();
    EXPECT_TRUE( success );
    EXPECT_EQ( storage.called_cnt, 568 );
    EXPECT_EQ( bigraph_cur_size, 5113 );
}


TEST( test_data, trigraph_AVv1_facts )
{
    count_calls_storage_mock storage{};
    enigma::string_dict_loader dl{ "%3s%d", storage, trigraph_AVv1 };

    bool success = dl.load();
    EXPECT_TRUE( success );
    EXPECT_EQ( storage.called_cnt, 4277 );
    EXPECT_EQ( trigraph_AVv1_size, 47048 );
}


TEST( test_data, trigraph_cur_facts )
{
    count_calls_storage_mock storage{};
    enigma::string_dict_loader dl{ "%3s%d", storage, trigraph_cur };

    bool success = dl.load();
    EXPECT_TRUE( success );
    EXPECT_EQ( storage.called_cnt, 3514 );
    EXPECT_EQ( trigraph_cur_size, 28558 );
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
