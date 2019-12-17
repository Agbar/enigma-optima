cmake_minimum_required(VERSION 3.14)

project(enigma_optima_suite 
    VERSION 1.1.1
    HOMEPAGE_URL https://github.com/Agbar/enigma-optima
    LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug RelWithDebInfo")
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    # set_property accepts a semicolon separated list
    set(CMAKE_CONFIGURATION_TYPE_LIST "${CMAKE_CONFIGURATION_TYPES}")
    separate_arguments(CMAKE_CONFIGURATION_TYPE_LIST)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPE_LIST})
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

include(CheckPIESupported)
check_pie_supported(
    LANGUAGES CXX
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(ENIGMA_BUILD_BITNESS "x64" CACHE STRING "Build artifacts bintess")
    set(BITNESS_TYPES x64 x86 x32)
    set_property(CACHE ENIGMA_BUILD_BITNESS PROPERTY STRINGS ${BITNESS_TYPES})
    
    if(ENIGMA_BUILD_BITNESS IN_LIST BITNESS_TYPES)
        set(ENIGMA_BITS_DIRECTORY_NAME ${ENIGMA_BUILD_BITNESS})
    else()
        message(WARNING "Not recognized Enigma Bitness value: ${ENIGMA_BUILD_BITNESS}")
    endif()
else()
    set(ENIGMA_BITS_DIRECTORY_NAME "")
endif()


if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)
    if(ENIGMA_BUILD_BITNESS STREQUAL "x86")
        add_compile_options(-m32 -march=pentium3)
        add_link_options(-m32)
        # Disable position independent code generation
        # * default on Windows
        # * without it Linux code is significantly slower
        add_compile_options(-fno-pic -fno-pie)
        # Setting CMAKE_POSITION_INDEPENDENT_CODE=FALSE breaks benchmark
        # which requires cmake 2.8; -no-pie is added by 3.14+
        if(CMAKE_CXX_LINK_NO_PIE_SUPPORTED)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
        endif()
    elseif(ENIGMA_BUILD_BITNESS STREQUAL "x32")
        add_compile_options(-mx32 -march=x86-64)
        add_link_options(-mx32)
        # Disable position independent code generation
        # * default on Windows
        # * without it Linux code is significantly slower
        add_compile_options(-fno-pic -fno-pie)
        if(CMAKE_CXX_LINK_NO_PIE_SUPPORTED)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
        endif()
    elseif(ENIGMA_BUILD_BITNESS STREQUAL "x64")
        # Official MinGW-w64 builds have changed arch
        add_compile_options(-m64 -march=x86-64)
        # add_link_options in CMake 3.13
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
    endif()
    # Official MinGW-w64 builds have changed tune
    add_compile_options(-mtune=generic)

    check_c_compiler_flag(-Wimplicit-fallthrough=5 GCC_HAS_IMPLICIT_FALLTHROUGH)
    if(GCC_HAS_IMPLICIT_FALLTHROUGH)
        list(APPEND ENIGMA_COMPILE_OPTIONS -Wimplicit-fallthrough=5)        
    endif()

    # treat some warnings as errors
    list(APPEND ENIGMA_COMPILE_OPTIONS
        -Werror=discarded-qualifiers
        -Werror=implicit-function-declaration
        -Werror=incompatible-pointer-types
        -Werror=missing-braces
        -Werror=return-type
        -Werror=non-virtual-dtor
    )
    # add some warnings
    list(APPEND ENIGMA_COMPILE_OPTIONS
        -Wall
        -Wcast-align
        -Wextra
        -Wfloat-equal
        -Wmissing-declarations
        -Wredundant-decls
        -Wshadow
        -Wunsafe-loop-optimizations
        -Wvector-operation-performance)
    # and change call convention for all targets
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

    set(ENIGMA_CXX_COMPILE_OPTIONS
        -Wnon-virtual-dtor)
endif()

find_package(Git REQUIRED)
execute_process(COMMAND ${GIT_EXECUTABLE} describe --dirty 
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE ENIGMA_GIT_DESCRIBE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE ENIGMA_GIT_DESCRIBE_RESULT)
if(NOT ENIGMA_GIT_DESCRIBE_RESULT EQUAL 0)
    message(WARNING "`git describe` failed")
endif()


add_subdirectory(libEnigma)

add_subdirectory(optima)

add_subdirectory(benchmark)

add_subdirectory(BOINC)

enable_testing()
add_subdirectory(tests)

set(BENCHMARK_BUILD_32_BITS ${ENIGMA_BUILD_32_BIT} CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "")
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
add_subdirectory(extern/gbench)

set(INSTALL_GTEST OFF CACHE BOOL "")
set(BUILD_GMOCK OFF CACHE BOOL "")
add_subdirectory(extern/gtest)

add_subdirectory(archive)
