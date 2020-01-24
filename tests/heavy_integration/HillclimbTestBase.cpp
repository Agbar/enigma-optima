#include "HillclimbTestBase.hpp"

#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>


extern "C"{
#include "SwMode.h"
#include "ciphertext.h"
#include "enigma/test_data.h"
#include "input.h"
}

namespace heavy_test {

static void load_message( size_t s,  const char text[] ){
    for( size_t i = 0 ; i < s ; ++i ) {
        ciphertext.plain[i] = make_echar_ascii( text[i] );
    }
}

}


void HillclimbTestBase::LoadMessage( int& length ) {
    heavy_test::load_message( benchmark_cipher_pbnxa_size, benchmark_cipher_pbnxa );
    length = benchmark_cipher_pbnxa_size - 1;
    if( length > CT ) length = CT;
}


void HillclimbTestBase::ClearMessage( ) {
    std::memset( ciphertext.plain, 0, sizeof ciphertext.plain );
}


void HillclimbTestBase::SetKeyRange( Key& from, Key& to ) {
    ASSERT_TRUE( set_range( &from,
                            &to,
                            "B:532:AA:AAA",
                            "B:532:AH:ZZZ",
                            EnigmaModel_M3 ) );
}


void HillclimbTestBase::SetUp() {
    if( active ) throw std::logic_error( "another test is active." );
    active = this;

    enigma_cipher_function_t* cipher_fun;
    std::tie( cipher_fun, scOptimizer.score_impl, isSupportedFun ) = GetParam();
    scOptimizer.prepare_decoder_lookup = cipher_fun->prepare_decoder_lookup_M_H3;
    scOptimizer.optimize_score = &GetStbrettOptimizer();
}


void HillclimbTestBase::TearDown() {
    active = nullptr;
}


void HillclimbTestBase::RunHillclimb() {
    // make the test deterministic
    srand( 42 );

    Key ckey = from;
    gkey = from;
    State state = {
        from : &from,
        to : &to,
        ckey : &ckey, // not a resume
        gkey : &gkey, // not a resume
        sw_mode : {SwMode::SW_ONSTART},
        pass : 0,
        firstpass : true,
        ciphertext : ciphertext.plain
    };
    knapsack.scrambler_state_is_endloop = select_scrambler_state_is_endloop_impl( &state );
    hillclimb( &state,
                /* max_pass */   1,
                len,
                &knapsack );
}


void HillclimbTestBase::RunAssertions() const {
    RunFinalAssertions();
}


Key HillclimbTestBase::from;
Key HillclimbTestBase::to;
int HillclimbTestBase::len;
HillclimbTestBase* HillclimbTestBase::active;
