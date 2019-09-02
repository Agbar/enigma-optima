# Make archive to be published.


find_program(ENIGMA_7Z_EXECUTABLE
    7z
    DOC "Path to 7z archiver"
    HINTS
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\7-Zip;Path64]
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\7-Zip;Path]
)


if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(ENIGMA_WRAPPER_BINDIR_PATH "wrapper/wrapper_5.32_i686-pc-linux-gnu")
    set(ENIGMA_WRAPPER_FILE "${CMAKE_BINARY_DIR}/${ENIGMA_WRAPPER_BINDIR_PATH}")

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
    set(ENIGMA_WRAPPER_BINDIR_PATH "wrapper/wrapper_5.32_windows_intelx86.exe")
    set(ENIGMA_WRAPPER_FILE "${CMAKE_BINARY_DIR}/${ENIGMA_WRAPPER_BINDIR_PATH}")

    file(DOWNLOAD
        http://download.enigmaathome.net/wrapper_5.32_windows_intelx86.exe
        ${ENIGMA_WRAPPER_FILE}
        SHOW_PROGRESS
        EXPECTED_HASH SHA256=3ef5e0f458b8cb0fdd286121db8b7c8a891db1a4789bdf6e2156443ed090f30c
        TLS_VERIFY on
    )

    set(ENIGMA_WRAPPER_2_BINDIR_PATH "wrapper/wrapper_1.05_windows_x86_64.exe")
    set(ENIGMA_WRAPPER_FILE_2 "${CMAKE_BINARY_DIR}/${ENIGMA_WRAPPER_2_BINDIR_PATH}")
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
    COMMAND ${ENIGMA_7Z_EXECUTABLE} u
        -stl -saa -ms=on -mx=9
        ${ENIGMA_ARCHIVE_FILE_NAME}
        # files
            $<TARGET_FILE:EnigmaOptima>
            $<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:EnigmaOptima>/$<TARGET_FILE_PREFIX:EnigmaOptima>$<TARGET_FILE_BASE_NAME:EnigmaOptima>.pdb>
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


set(ENIGMA_ARCHIVE_ALL_FILE_NAME "enigma-optima-all-${ENIGMA_GIT_DESCRIBE_VERSION}")
string(APPEND ENIGMA_ARCHIVE_ALL_FILE_NAME "+${CMAKE_SYSTEM_NAME}.${ENIGMA_BUILD_BITNESS}")
string(APPEND ENIGMA_ARCHIVE_ALL_FILE_NAME ".${CMAKE_C_COMPILER_ID}.${CMAKE_C_COMPILER_VERSION}")


set(ENIGMA_ARCHIVE_ALL_DIR "${CMAKE_BINARY_DIR}/archive-all")
file(MAKE_DIRECTORY "${ENIGMA_ARCHIVE_ALL_DIR}")

add_custom_target(ArchiveAll
    WORKING_DIRECTORY
        ${CMAKE_BINARY_DIR}
    COMMAND ${ENIGMA_7Z_EXECUTABLE} u
        -stl -saa -ms=on -mx=9
        ${ENIGMA_ARCHIVE_ALL_DIR}/${ENIGMA_ARCHIVE_ALL_FILE_NAME}
        # Keep a folder structure except the LICENSE file.
        # files
            ${CMAKE_SOURCE_DIR}/LICENSE
        # optima
            optima/$<TARGET_FILE_NAME:EnigmaOptima>
            $<$<PLATFORM_ID:Windows>:optima/$<TARGET_FILE_PREFIX:EnigmaOptima>$<TARGET_FILE_BASE_NAME:EnigmaOptima>.pdb>
        # BOINC
            BOINC/*.xml
        # wrapper
            ${ENIGMA_WRAPPER_BINDIR_PATH}
            ${ENIGMA_WRAPPER_2_BINDIR_PATH}
        # libEnigma
            libEnigma/$<TARGET_FILE_NAME:Enigma>
        # tests
            tests/$<TARGET_FILE_NAME:EnigmaUnitTests>
            $<$<PLATFORM_ID:Windows>:tests/libwinpthread-1.dll>
        # benchmark
            benchmark/$<TARGET_FILE_NAME:EnigmaBenchmark>
            benchmark/00bigr.cur
            benchmark/00trigr.AVv1
            benchmark/00unigr.AVv1
            $<$<PLATFORM_ID:Windows>:benchmark/libwinpthread-1.dll>

    BYPRODUCTS ${ENIGMA_ARCHIVE_ALL_DIR}/${ENIGMA_ARCHIVE_ALL_FILE_NAME}.7z
    DEPENDS
        ${CMAKE_BINARY_DIR}/BOINC/*.xml
        ${ENIGMA_WRAPPER_FILE}
        ${CMAKE_SOURCE_DIR}/LICENSE
)

add_dependencies(ArchiveAll
    Enigma
    EnigmaOptima
    EnigmaUnitTests
    EnigmaBenchmark
)
