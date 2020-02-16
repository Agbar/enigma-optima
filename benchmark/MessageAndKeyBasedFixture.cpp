#include "MessageAndKeyBasedFixture.h"

extern "C"{
#include "enigma/test_data.h"
#include "ciphertext.h"
#include "input.h"
#include "stecker.h"
}

#include "load/strload_error.hpp"

using rt = ::ring_type_enum;

void
MessageAndKeyBasedFixture::SetUp( benchmark::State& st )
{
    try {
        LoadDictionary();
    }
    catch(enigma::strload_error r) {
        st.SkipWithError( r.what() );
        return;
    }

    // Score: 46438
    // UKW: B       // ukwnum
    // W/0: 532     // slot
    // Stecker: AXBZCNDPEOJMLSQV
    // Rings: AAA
    // Message key: AVA

    for( size_t i = 0 ; i < benchmark_cipher_pbnxa_size ; ++i )
    {
        ciphertext.plain[i] = make_echar_ascii( benchmark_cipher_pbnxa[i] );
    }

    key.slot = { /*.g:*/ {}, /*.l:*/ {rt::RingType_5}, /*.m:*/ {rt::RingType_3}, /*.r:*/ {rt::RingType_2} };
    struct echar_delta 
        dA = make_echar_delta_ascii( 'A' ),
        dV = make_echar_delta_ascii( 'V' );
    key.ring = {m : dA, r : dA};                    // AAA
    key.mesg = {g : {}, l : dA, m : dV, r : dA};    // AVA
    key.ukwnum.type = UkwType_B;
    key.model = EnigmaModel_M3;

    Fill0To25_echar( key.stbrett.letters );
    Fill0To25_echar( key.sf.map );
    set_stecker( &key, "AXBZCNDPEOJMLSQV" );
    get_stecker( &key );

    len = benchmark_cipher_pbnxa_size - 1;
}
