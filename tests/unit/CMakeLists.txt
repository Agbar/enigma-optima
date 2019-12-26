set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include(FindThreads)

add_executable(EnigmaUnitTests "main.cpp")
target_sources(EnigmaUnitTests
PRIVATE
    "cipher.tests.cpp"
    "computeScoreNoInterleave.cpp"
    "GetRSeed.tests.cpp"
    "string_dict_loader.tests.cpp"
    "test_data.tests.cpp"
)
include("echar/echar.cmake")
include("x86/x86.cmake")

target_compile_features(EnigmaUnitTests PUBLIC cxx_std_11 c_std_99)
target_compile_options(EnigmaUnitTests PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
target_compile_options(EnigmaUnitTests
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${ENIGMA_CXX_COMPILE_OPTIONS}>
        ${ENIGMA_COMPILE_OPTIONS})

target_link_libraries(EnigmaUnitTests Enigma EnigmaTestData gtest Threads::Threads)

add_test(
    NAME Units
    COMMAND EnigmaUnitTests)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    include(FindLibWinThreadDll)
    file(CREATE_LINK
        "${ENIGMA_WINPTHREAD_DLL_FILE}"
        "${CMAKE_CURRENT_BINARY_DIR}/libwinpthread-1.dll"
        COPY_ON_ERROR
    )
endif()