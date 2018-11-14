cmake_minimum_required(VERSION 3.1)

add_library(Enigma "")

include(commonSources.cmake)
if(WIN32)
    target_sources(Enigma PRIVATE "src/randomNumbers.Windows.c")
elseif(UNIX)
    target_sources(Enigma PRIVATE "src/randomNumbers.Linux.c")
endif()

target_include_directories(Enigma PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_features(Enigma PUBLIC c_std_99)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")    
    if(ENIGMA_BUILD_32_BIT)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -march=pentium3")
        
        # Disable position independent code generation
        # * default on Windows
        # * without it Linux code is significantly slower
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-pic -fno-pie")
    else()
        # Official MinGW-w64 builds have changed arch
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -march=x86-64")
    endif()
    # Official MinGW-w64 builds have changed tune
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=generic")

    include(CheckCCompilerFlag)
    check_c_compiler_flag(-Wimplicit-fallthrough=5 GCC_HAS_IMPLICIT_FALLTHROUGH)
    if(GCC_HAS_IMPLICIT_FALLTHROUGH)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wimplicit-fallthrough=5")
    endif()

    # treat some warnings as errors
    set(ENIGMA_ERROR_WARNING_FLAGS "-Werror=implicit-function-declaration -Werror=return-type")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ENIGMA_ERROR_WARNING_FLAGS}")

    set(ENIGMA_WARNING_FLAGS "-Wall -Wcast-align -Wextra -Wfloat-equal -Wmissing-declarations -Wredundant-decls -Wshadow -Wunsafe-loop-optimizations")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ENIGMA_WARNING_FLAGS}")

    set(ENIGMA_CUSTOM_CALL_CONVENTION_FLAGS "-fcall-used-xmm6 -fcall-used-xmm7 -fcall-used-xmm8 -fcall-used-xmm9 -fcall-used-xmm10 -fcall-used-xmm11 -fcall-used-xmm12 -fcall-used-xmm13 -fcall-used-xmm14 -fcall-used-xmm15")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ENIGMA_CUSTOM_CALL_CONVENTION_FLAGS}")
endif()