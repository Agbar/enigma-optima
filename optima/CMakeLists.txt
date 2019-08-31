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


if(CMAKE_SYSTEM_NAME STREQUAL Windows)

    find_program(ENIGMA_CV2PDB_EXECUTABLE
        cv2pdb
        DOC "Path to debugging info converter provided with Visual D."
        HINTS
            [HKEY_LOCAL_MACHINE\\SOFTWARE\\VisualD;]\\cv2pdb
            [HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\VisualD;]\\cv2pdb
    )

    get_filename_component(VS10_INSTALL_DIR
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\10.0;InstallDir]
        ABSOLUTE
    )
    string(REPLACE "/" "\\" VS10_INSTALL_DIR "${VS10_INSTALL_DIR}")

    add_custom_command(
        COMMENT "Convert debug info to PDB"
        TARGET EnigmaOptima
        POST_BUILD
        COMMAND set "PATH=${VS10_INSTALL_DIR};%PATH%"
            && ${ENIGMA_CV2PDB_EXECUTABLE}
                -C
                $<TARGET_FILE:EnigmaOptima>
    )

endif()