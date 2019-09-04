if(NOT ENIGMA_WINPTHREAD_DLL_FILE)
    execute_process(
        COMMAND
            ${CMAKE_C_COMPILER} -print-file-name=libwinpthread-1.dll
        OUTPUT_VARIABLE WINPTHREAD_FILE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE ENIGMA_WINPTHREAD_DLL_PRINT_FILE_NAME_RESULT
    )
    if(NOT ENIGMA_WINPTHREAD_DLL_PRINT_FILE_NAME_RESULT EQUAL 0)
        message(WARNING "libwinpthread-1.dll not found" )
    else()
        set(
            ENIGMA_WINPTHREAD_DLL_FILE
            "${WINPTHREAD_FILE}"
            CACHE
            FILEPATH
            "A path to winthread dll required to run tests. Used in ArchiveAll."
        )
    endif()
endif()
