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
