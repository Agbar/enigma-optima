#pragma once

#include <cstddef>
#include <algorithm>
#include <iterator>
#include <utility>

template< class TEcharVector, size_t text_len >
TEcharVector make_v( const char (&text)[text_len] ){
    static_assert( sizeof(TEcharVector) + 1 == text_len, "length of text must match vector size" );
    TEcharVector r;
    std::transform(
        std::begin( text ),
        std::min( std::begin( text ) + sizeof( r.v_ec ), std::end( text ) ),
        std::begin( r.v_ec ),
        [](const char &a) noexcept { return make_echar_ascii( a ); } );
    return r;
}

