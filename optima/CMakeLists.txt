add_executable(EnigmaOptima "enigma.c")

target_sources(EnigmaOptima
PRIVATE
    "banner.c"
    "display.c"
    "error.c"
    "config/releaseVersion.cpp"
    "OS/OsDefault.c"
    "OS/OsWindows.c"
)
set_target_properties(EnigmaOptima
    PROPERTIES OUTPUT_NAME "enigma-optima")

target_compile_features(EnigmaOptima PUBLIC c_std_99 cxx_std_11)
target_compile_options(EnigmaOptima
    PRIVATE
        ${ENIGMA_COMPILE_OPTIONS})


if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set_target_properties(EnigmaOptima PROPERTIES C_EXTENSIONS ON)
endif()

target_link_libraries(EnigmaOptima Enigma)

configure_file (
    "config/releaseVersion.gen.hpp.in"
    "config/releaseVersion.gen.hpp"
)
target_include_directories(EnigmaOptima
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/config>
)
