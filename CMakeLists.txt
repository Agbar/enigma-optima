cmake_minimum_required(VERSION 3.0)

project(enigma_optima_suite LANGUAGES C CXX)

set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug RelWithDebInfo")
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${CMAKE_CONFIGURATION_TYPES}")
endif()

# Substitute default flags (use -O3 instead of -O2)
if(CMAKE_C_COMPILER_ID MATCHES "GNU"
    AND CMAKE_CXX_COMPILER_ID MATCHES "GNU"
    AND NOT ENIGMA_DEFAULT_FLAGS_SET)
    if(NOT ENIGMA_DEFAULT_FLAGS_RELWITHDEBINFO)
        set(ENIGMA_DEFAULT_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG")
    endif()
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${ENIGMA_DEFAULT_FLAGS_RELWITHDEBINFO}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${ENIGMA_DEFAULT_FLAGS_RELWITHDEBINFO}" CACHE STRING "" FORCE)
    set(ENIGMA_DEFAULT_FLAGS_SET ON CACHE BOOL "Whether C/CXX_FLAGS_RELWITHDEBINFO has been changed" FORCE)
    mark_as_advanced(ENIGMA_DEFAULT_FLAGS_SET)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    option(ENIGMA_BUILD_32_BIT "Build 32 bit artifacts (x86)" OFF)
    if(ENIGMA_BUILD_32_BIT)
        set(ENIGMA_BITS_DIRECTORY_NAME "/x86")
    else()
        set(ENIGMA_BITS_DIRECTORY_NAME "/x64")
    endif()
else()
    set(ENIGMA_BITS_DIRECTORY_NAME "")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/lib/${ENIGMA_BITS_DIRECTORY_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/lib/${ENIGMA_BITS_DIRECTORY_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin/${ENIGMA_BITS_DIRECTORY_NAME}")

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    if(ENIGMA_BUILD_32_BIT)
        add_compile_options(-m32 -march=pentium3)
        # Disable position independent code generation
        # * default on Windows
        # * without it Linux code is significantly slower
        add_compile_options(-fno-pic -fno-pie)
        # add_link_options in CMake 3.13
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32 -no-pie")
    else()
        # Official MinGW-w64 builds have changed arch
        add_compile_options(-m64 -march=x86-64)
        # add_link_options in CMake 3.13
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
    endif()
    # Official MinGW-w64 builds have changed tune
    add_compile_options(-mtune=generic)

    include(CheckCCompilerFlag)
    check_c_compiler_flag(-Wimplicit-fallthrough=5 GCC_HAS_IMPLICIT_FALLTHROUGH)
    if(GCC_HAS_IMPLICIT_FALLTHROUGH)
        add_compile_options(-Wimplicit-fallthrough=5)
    endif()

    # treat some warnings as errors
    add_compile_options(
        -Werror=implicit-function-declaration
        -Werror=return-type)
    # add some warnings
    add_compile_options(
        -Wall
        -Wcast-align
        -Wextra
        -Wfloat-equal
        -Wmissing-declarations
        -Wredundant-decls
        -Wshadow
        -Wunsafe-loop-optimizations
        -Wvector-operation-performance)
    # and change call convention
    add_compile_options(
        -fcall-used-xmm6
        -fcall-used-xmm7
        -fcall-used-xmm8
        -fcall-used-xmm9
        -fcall-used-xmm10
        -fcall-used-xmm11
        -fcall-used-xmm12
        -fcall-used-xmm13
        -fcall-used-xmm14
        -fcall-used-xmm15)
endif()

add_subdirectory(libEnigma)

add_subdirectory(optima)

add_subdirectory(benchmark)

enable_testing()
add_subdirectory(tests)
