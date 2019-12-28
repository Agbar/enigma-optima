#pragma GCC target ("avx2")

#include <algorithm>
#include <string>

#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar.h"
    #include "echar/echar_avx2.h"
}

#include "v_tools.hpp"

TEST( v32_echar__PermutationMap, v32_echar_map ){
    if( !__builtin_cpu_supports("avx2") ) {
        return;
    }
    PermutationMap_t map;
    for( size_t i = 0; i < sizeof(map.letters); i++ ){
        map.letters[i] = make_echar( i % 26 );
    }

    const auto in = make_v<v32_echar>( "qwertyuiopasdfghjklzxcvbnmqwerty" );
    const v32_echar out = v32_echar_map( in, map );
    for( size_t j = 0; j < sizeof(out.vector); j++ )
    {
        EXPECT_EQ( in.vector[j], out.vector[j] ) << "Vectors differ at index " <<  j;
    }
}

TEST( v32_echar__PermutationMap, map_rot1 ){
    if( !__builtin_cpu_supports("avx2") ) {
        return;
    }
    PermutationMap_t map;
    for( size_t i = 0; i < sizeof(map.letters); i++ )
    {
        map.letters[i] = make_echar( ( i + 1 ) % 26 );
    }

    const auto in       = make_v<v32_echar>("qwertyuiopasdfghjklzxcvbnmqazwsx");
    const auto expected = make_v<v32_echar>("rxfsuzvjpqbteghiklmaydwconrbaxty");

    const v32_echar out = v32_echar_map( in, map );
    
    for( size_t j = 0; j < sizeof(out.vector); j++ )
    {
        EXPECT_EQ( expected.vector[j], out.vector[j] ) << "Vectors differ at index " << j;
    }
}