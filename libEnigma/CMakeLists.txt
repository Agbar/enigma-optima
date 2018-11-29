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
target_compile_options(Enigma
    PRIVATE
        ${ENIGMA_COMPILE_OPTIONS})


if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set_target_properties(Enigma PROPERTIES C_EXTENSIONS ON)
    target_link_libraries(Enigma
        -static-libgcc
        -static-libstdc++)
endif()