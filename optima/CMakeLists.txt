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


find_package(Git REQUIRED)
execute_process(COMMAND ${GIT_EXECUTABLE} describe --dirty 
    OUTPUT_VARIABLE ENIGMA_GIT_DESCRIBE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/config/releaseVersion.cpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/config/releaseVersion.cpp"
)
