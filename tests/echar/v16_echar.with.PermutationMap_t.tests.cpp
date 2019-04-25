#pragma GCC target("ssse3")

// #ifndef __SSSE3__
// # error SSSE3 not defined
// #endif

#include <algorithm>
#include <string>

#include "gtest/gtest.h"

extern "C"
{
#include "echar/echar.h"
#include "echar/echar_ssse3.h"
}

#include "v_tools.hpp"

TEST( v16_echar__PermutationMap, map_identity ){
    if( !__builtin_cpu_supports("ssse3") ) {
        return;
    }
    PermutationMap_t map;
    for( size_t i = 0; i < sizeof(map.letters); i++ )
    {
        map.letters[i] = make_echar(i % 26);
    }

    init_charmap();

    const auto in1 = make_v<v16_echar>( "qwertyuiopasdfgh" );
    const auto in2 = make_v<v16_echar>( "jklzxcvbnmfedcba" );
    const v16_echar out1 = v16_echar_map( in1, map );
    const v16_echar out2 = v16_echar_map( in2, map );
    
    for( size_t j = 0; j < sizeof(out1.vector); j++ )
    {
        EXPECT_EQ( in1.vector[j], out1.vector[j] ) << "Vectors differ at index " << j;
        EXPECT_EQ( in2.vector[j], out2.vector[j] ) << "Vectors differ at index " << j;
    }
}

TEST( v16_echar__PermutationMap, map_rot1 ){
    if( !__builtin_cpu_supports("ssse3") ) {
        return;
    }
    PermutationMap_t map;
    for( size_t i = 0; i < sizeof(map.letters); i++ )
    {
        map.letters[i] = make_echar( ( i + 1 ) % 26 );
    }

    init_charmap();

    const auto in1    = make_v<v16_echar>( "qwertyuiopasdfgh" );
    const auto expct1 = make_v<v16_echar>( "rxfsuzvjpqbteghi" );
    const auto in2    = make_v<v16_echar>( "jklzxcvbnmfedcba" );
    const auto expct2 = make_v<v16_echar>( "klmaydwcongfedcb" );

    const v16_echar out1 = v16_echar_map( in1, map );
    const v16_echar out2 = v16_echar_map( in2, map );
    
    for( size_t j = 0; j < sizeof(out1.vector); j++ )
    {
        EXPECT_EQ( expct1.vector[j], out1.vector[j] ) << "Vectors differ at index " << j;
        EXPECT_EQ( expct2.vector[j], out2.vector[j] ) << "Vectors differ at index " << j;
    }
}
