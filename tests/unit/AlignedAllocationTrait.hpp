#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

template <typename T>
struct AlignedAllocationTrait
{
    static void* operator new( std::size_t size ){
        void* ptr;
#if defined _WIN32
        ptr = _aligned_malloc( size , alignof(T) );
#else
        ptr = aligned_alloc( alignof(T), size );
#endif
        if( !ptr ) throw std::bad_alloc();
        return ptr;
    }

    static void operator delete( void* ptr ){
#if defined _WIN32
        _aligned_free( ptr );
#else
        free( ptr );
#endif
    }
};
