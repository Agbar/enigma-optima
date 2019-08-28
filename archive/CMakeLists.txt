# Make archive to be published.

file(DOWNLOAD
    http://download.enigmaathome.net/wrapper_5.32_i686-pc-linux-gnu
    "${CMAKE_BINARY_DIR}/wrapper/wrapper_5.32_i686-pc-linux-gnu"
    SHOW_PROGRESS
    EXPECTED_HASH SHA256=876f1c70297eeb5adca1f441923f05e6c0092f2fca232b4dcfddc5f3e6e4934a
    TLS_VERIFY on
)

# set +x to capture it in the archive
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    execute_process(COMMAND chmod ug+x "${CMAKE_BINARY_DIR}/wrapper/wrapper_5.32_i686-pc-linux-gnu")
endif()


set(ENIGMA_ARCHIVE_FILE_NAME "enigma-optima-${ENIGMA_GIT_DESCRIBE_VERSION}")
string(APPEND ENIGMA_ARCHIVE_FILE_NAME "+${CMAKE_SYSTEM_NAME}.${ENIGMA_BUILD_BITNESS}")
string(APPEND ENIGMA_ARCHIVE_FILE_NAME ".${CMAKE_C_COMPILER_ID}.${CMAKE_C_COMPILER_VERSION}")


add_custom_target(Archive
    COMMAND 7z u
        -stl -saa -ms=on
        ${ENIGMA_ARCHIVE_FILE_NAME}
        # files
            $<TARGET_FILE:EnigmaOptima>
            ${CMAKE_BINARY_DIR}/BOINC/*.xml
            ${CMAKE_BINARY_DIR}/wrapper/wrapper_5.32_i686-pc-linux-gnu
            ${CMAKE_SOURCE_DIR}/LICENSE

    BYPRODUCTS ${ENIGMA_ARCHIVE_FILE_NAME}.7z
    DEPENDS ${CMAKE_BINARY_DIR}/BOINC/*.xml ${CMAKE_BINARY_DIR}/wrapper/wrapper_5.32_i686-pc-linux-gnu ${CMAKE_SOURCE_DIR}/LICENSE
)

add_dependencies(Archive EnigmaOptima)
