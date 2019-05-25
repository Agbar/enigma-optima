#pragma once

inline
bool isSse2Supported() {
    return __builtin_cpu_supports( "sse2" );
}

inline
bool isSse2VexSupported() {
    return __builtin_cpu_supports( "sse2" )
        && __builtin_cpu_supports( "avx" );
}

inline 
bool isAvx2Supported() {
    return __builtin_cpu_supports( "avx2" );
}