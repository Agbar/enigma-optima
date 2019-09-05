function(find_gcc_file_name
        OUTPUT_FILE_VARIABLE
        DOC_STRING)
    if(OUTPUT_FILE)
        return()
    endif()
    execute_process(
        COMMAND
            ${CMAKE_C_COMPILER} -print-file-name=libwinpthread-1.dll
        OUTPUT_VARIABLE     WINPTHREAD_FILE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE     PRINT_FILE_NAME_RESULT
    )
    if(NOT PRINT_FILE_NAME_RESULT EQUAL 0)
        message(WARNING "libwinpthread-1.dll not found" )
        return()
    endif()
    set(
        ${OUTPUT_FILE_VARIABLE}
        "${WINPTHREAD_FILE}"
        CACHE
        FILEPATH
        ${DOC_STRING}
    )
    mark_as_advanced(FORCE ${OUTPUT_FILE_VARIABLE})
endfunction()

find_gcc_file_name(
    ENIGMA_WINPTHREAD_DLL_FILE
    "A path to winthread dll required to run tests. Used in ArchiveAll."
)
