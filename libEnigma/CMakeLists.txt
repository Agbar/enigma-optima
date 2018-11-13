cmake_minimum_required(VERSION 3.1)

project(Enigma LANGUAGES C)

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
    option(ENIGMA_BUILD_32_BIT "Build 32 bit artifacts (x86)" OFF)
    if(ENIGMA_BUILD_32_BIT)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -march=pentium3")
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

endif()