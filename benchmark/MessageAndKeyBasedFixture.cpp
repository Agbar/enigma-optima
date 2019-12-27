#include "MessageAndKeyBasedFixture.h"

extern "C"{
#include "charmap.h"
#include "ciphertext.h"
#include "input.h"
#include "stecker.h"
}

#include "load/strload_error.hpp"

using rt = RingType::ring_type_enum;

void
MessageAndKeyBasedFixture::SetUp( benchmark::State& st )
{
    init_charmap();
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

    const unsigned char ct[] = "pbnxasmdaxnooyhrczgvvzcbigibgwhm"
                               "xkrrvqcfjczptunswaddstigqqcsagpk"
                               "rxxlomgfxaphhmrfsdkytmypmvrohasq"
                               "yrwfwvavgccudbibxxdyzsacjsyotmwu"
                               "cnwomhhjpywdcclupgswclmbczssyxpg"
                               "mgmqxaufulnozeqenheizzaklc";

    for( size_t i = 0 ; i < sizeof ct; ++i )
    {
        ciphertext.plain[i] = make_echar_ascii( ct[i] );
    }

    key.slot = { /*.g:*/ {}, /*.l:*/ {rt::RingType_5}, /*.m:*/ {rt::RingType_3}, /*.r:*/ {rt::RingType_2} };
    struct echar_delta 
        dA = make_echar_delta_ascii( 'A' ),
        dV = make_echar_delta_ascii( 'V' );
    key.ring = { /*.g:*/ {}, /*.l:*/ dA, /*.m:*/ dA, /*.r:*/ dA }; // AAA
    key.mesg = { /*.g:*/ {}, /*.l:*/ dA, /*.m:*/ dV, /*.r:*/ dA }; // AVA
    key.ukwnum.type = UkwType::UkwType_B;
    key.model = EnigmaModel_M3;

    Fill0To25_echar( key.stbrett.letters );
    Fill0To25_echar( key.sf.map );
    char stecker[] = "AXBZCNDPEOJMLSQV";
    set_stecker( &key, stecker );
    get_stecker( &key );

    len = sizeof ct - 1;
}
