#include "MessageAndKeyBasedFixture.h"

extern "C"{
#include "charmap.h"
#include "ciphertext.h"
#include "input.h"
#include "stecker.h"
}

using rt = RingType::ring_type_enum;

void
MessageAndKeyBasedFixture::SetUp( benchmark::State& st UNUSED )
{
    init_charmap();

    LoadDictionary();

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
        ciphertext.plain[i] = make_echar( code[ct[i]] );
    }

    key.slot = { /*.g:*/ {}, /*.l:*/ {rt::RingType_5}, /*.m:*/ {rt::RingType_3}, /*.r:*/ {rt::RingType_2} };
    key.ring = { /*.g:*/ 0, /*.l:*/ 0, /*.m:*/0, /*.r:*/ 0 }; // AAA
    key.mesg = { /*.g:*/ 0, /*.l:*/ 0, /*.m:*/'V' - 'A', /*.r:*/ 0 }; // AVA
    key.ukwnum = 'B' - 'A';
    key.model = EnigmaModel_M3;

    Fill0To25_echar( key.stbrett.letters );
    Fill0To25_echar( key.sf.map );
    char stecker[] = "AXBZCNDPEOJMLSQV";
    set_stecker( &key, stecker );
    get_stecker( &key );

    len = sizeof ct - 1;
}
