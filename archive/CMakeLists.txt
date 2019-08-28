# Make archive to be published.


if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(ENIGMA_WRAPPER_FILE
        "${CMAKE_BINARY_DIR}/wrapper/wrapper_5.32_i686-pc-linux-gnu")

    file(DOWNLOAD
        http://download.enigmaathome.net/wrapper_5.32_i686-pc-linux-gnu
        ${ENIGMA_WRAPPER_FILE}
        SHOW_PROGRESS
        EXPECTED_HASH SHA256=876f1c70297eeb5adca1f441923f05e6c0092f2fca232b4dcfddc5f3e6e4934a
        TLS_VERIFY on
    )

    # set +x to capture it in the archive
    execute_process(COMMAND chmod ug+x ${ENIGMA_WRAPPER_FILE})
elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(ENIGMA_WRAPPER_FILE
        "${CMAKE_BINARY_DIR}/wrapper/wrapper_5.32_windows_intelx86.exe")

    file(DOWNLOAD
        http://download.enigmaathome.net/wrapper_5.32_windows_intelx86.exe
        ${ENIGMA_WRAPPER_FILE}
        SHOW_PROGRESS
        EXPECTED_HASH SHA256=3ef5e0f458b8cb0fdd286121db8b7c8a891db1a4789bdf6e2156443ed090f30c
        TLS_VERIFY on
    )

    set(ENIGMA_WRAPPER_FILE_2 ${CMAKE_BINARY_DIR}/wrapper/wrapper_1.05_windows_x86_64.exe)
    file(CREATE_LINK
        ${ENIGMA_WRAPPER_FILE}
        ${ENIGMA_WRAPPER_FILE_2}
        COPY_ON_ERROR
    )
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
            ${ENIGMA_WRAPPER_FILE}
            ${ENIGMA_WRAPPER_FILE_2}
            ${CMAKE_SOURCE_DIR}/LICENSE

    BYPRODUCTS ${ENIGMA_ARCHIVE_FILE_NAME}.7z
    DEPENDS
        ${CMAKE_BINARY_DIR}/BOINC/*.xml
        ${ENIGMA_WRAPPER_FILE}
        ${CMAKE_SOURCE_DIR}/LICENSE
)

add_dependencies(Archive EnigmaOptima)
