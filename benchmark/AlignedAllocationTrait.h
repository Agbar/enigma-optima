#pragma once

#include <cstddef>
#include <cstdlib>

template <typename T>
struct AlignedAllocationTrait
{
    static void* operator new( std::size_t size ){
        return aligned_alloc( alignof(T), size );
    }

    static void operator delete( void* ptr ){
        free( ptr );
    }
};
