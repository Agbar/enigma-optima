#include "Hillclimb_PBNXA.hpp"

#include <algorithm>
#include <gtest/gtest.h>


#include "dicts/dict_builders.hpp"
#include "dicts/string_dict_loader.hpp"


extern "C"{
#include "ciphertext.h"
#include "enigma/test_data.h"
#include "input.h"
}


static bool load_tridict( const char text[] ){
    enigma::tri_dict_builder storage{};
    enigma::string_dict_loader tri{ "%3s%d", storage, text };
    return tri.load();
}


static bool load_bidict( const char text[] ){
    enigma::bi_dict_builder storage{};
    enigma::string_dict_loader bi{ "%2s%d", storage, text };
    return bi.load();
}


static void load_message( size_t s,  const char text[] ){
    for( size_t i = 0 ; i < s ; ++i ) {
        ciphertext.plain[i] = make_echar_ascii( text[i] );
    }
}


void Hillclimb_PBNXA::SetUpTestCase(){
    // command line is:
    // enigma.exe -M M3 -c -o bench-result.txt -f "B:532:AA:AAA" -t
    // "B:532:AH:ZZZ" 00trigr.cur 00bigr.cur benchmark_cipher

    ASSERT_TRUE( load_tridict( trigraph_cur ) );
    ASSERT_TRUE( load_bidict( bigraph_cur ) );

    load_message( benchmark_cipher_pbnxa_size, benchmark_cipher_pbnxa );

    ASSERT_TRUE( set_range( &from,
                            &to,
                            "B:532:AA:AAA",
                            "B:532:AH:ZZZ",
                            EnigmaModel_M3 ) );
    len = benchmark_cipher_pbnxa_size - 1;
    if( len > CT ) len = CT;
}


void Hillclimb_PBNXA::SetUp() {
    std::tie( scOptimizer.prepare_decoder_lookup, scOptimizer.score_impl ) =GetParam();
}


void Hillclimb_PBNXA::RunHillclimb() {
    // make the test deterministic
    srand( 42 );

    Key ckey = from;
    gkey = from;
    State state = {
        from : &from,
        to : &to,
        ckey : &ckey, // not a resume
        gkey : &gkey, // not a resume
        sw_mode : SW_ONSTART,
        pass : 0,
        firstpass : true,
        max_score : INT_MAX - 1,
        ciphertext : ciphertext.plain
    };
    hillclimb( &state,
                /* max_pass */   1,
                len,
                &knapsack );
}


void Hillclimb_PBNXA::RunAssertions() {
    EXPECT_EQ( gkey.score, 17930 );

    EXPECT_EQ( gkey.ukwnum.type, UkwType::UkwType_B );
    auto& slot = gkey.slot;
    EXPECT_EQ( slot.l.type, RingType::RingType_5 );
    EXPECT_EQ( slot.m.type, RingType::RingType_3 );
    EXPECT_EQ( slot.r.type, RingType::RingType_2 );

    std::string stecker;
    std::transform(
        std::begin( gkey.sf.map ),
        std::begin( gkey.sf.map ) + gkey.count,
        std::back_inserter( stecker ),
        []( const echar& e ) { return echar_to_ALPHA( e ); } 
    );
    EXPECT_EQ( stecker, "AIBECJDRFYGOHZMUNQPVST" );

    char ring[]{
        echar_delta_to_ALPHA( gkey.ring.l ),
        echar_delta_to_ALPHA( gkey.ring.m ),
        echar_delta_to_ALPHA( gkey.ring.r ), '\0' };
    EXPECT_STREQ( ring, "AAC" );
    char mesg[]{
        echar_delta_to_ALPHA( gkey.mesg.l ),
        echar_delta_to_ALPHA( gkey.mesg.m ),
        echar_delta_to_ALPHA( gkey.mesg.r ), '\0' };
    EXPECT_STREQ( mesg, "HVS" );
}


Key Hillclimb_PBNXA::from;
Key Hillclimb_PBNXA::to;
int Hillclimb_PBNXA::len;
