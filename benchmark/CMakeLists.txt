cmake_minimum_required(VERSION 3.1)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include(FindThreads)

add_executable(EnigmaBenchmark "")
include(sourceList.cmake)

set_target_properties(EnigmaBenchmark
    PROPERTIES OUTPUT_NAME "enigma-optima-benchmark")

target_compile_features(EnigmaBenchmark PUBLIC cxx_std_11 c_std_99)
target_compile_options(EnigmaBenchmark PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
target_compile_options(EnigmaBenchmark
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${ENIGMA_CXX_COMPILE_OPTIONS}>
        ${ENIGMA_COMPILE_OPTIONS})


if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set_target_properties(EnigmaBenchmark PROPERTIES C_EXTENSIONS ON)
endif()


target_link_libraries(EnigmaBenchmark Enigma benchmark Threads::Threads)
target_link_libraries(EnigmaBenchmark
        -static-libgcc
        -static-libstdc++)

if(WIN32)
    target_link_libraries(EnigmaBenchmark shlwapi)
endif()

configure_file(00bigr.cur 00bigr.cur COPYONLY)
configure_file(00trigr.AVv1 00trigr.AVv1 COPYONLY)
configure_file(00unigr.AVv1 00unigr.AVv1 COPYONLY)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    include(FindLibWinThreadDll)
    file(CREATE_LINK
        "${ENIGMA_WINPTHREAD_DLL_FILE}"
        "${CMAKE_CURRENT_BINARY_DIR}/libwinpthread-1.dll"
        COPY_ON_ERROR
    )
endif()
