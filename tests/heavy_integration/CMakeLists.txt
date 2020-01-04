set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include(FindThreads)


add_executable(EnigmaHeavyIntegrationTests "main.cpp")
target_sources(EnigmaHeavyIntegrationTests
PRIVATE
    "win_bench_pbnxa.cpp"
    "HillclimbTestBase.cpp"
)


target_compile_features(EnigmaHeavyIntegrationTests PUBLIC cxx_std_11 c_std_99)
target_compile_options(EnigmaHeavyIntegrationTests PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
target_compile_options(EnigmaHeavyIntegrationTests
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${ENIGMA_CXX_COMPILE_OPTIONS}>
        ${ENIGMA_COMPILE_OPTIONS})


target_link_libraries(EnigmaHeavyIntegrationTests
    Enigma 
    EnigmaTestData
    gtest
    Threads::Threads)

add_test(
    NAME "Heavy_Integration"
    COMMAND EnigmaHeavyIntegrationTests)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    include(FindLibWinThreadDll)
    file(CREATE_LINK
        "${ENIGMA_WINPTHREAD_DLL_FILE}"
        "${CMAKE_CURRENT_BINARY_DIR}/libwinpthread-1.dll"
        COPY_ON_ERROR
    )
endif()
