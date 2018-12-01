set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include(FindThreads)

set(GOOGLE_TEST_INCLUDE_DIR "NOTFOUND" CACHE PATH "Google Test include dir")
set(GOOGLE_TEST_LIB_DIR "NOTFOUND" CACHE PATH "Google Test library dir")

link_directories(${GOOGLE_TEST_LIB_DIR})

add_executable(EnigmaUnitTests "main.cpp")
target_sources(EnigmaUnitTests
PRIVATE
    "cipher.tests.cpp"
    "GetRSeed.tests.cpp"
)

target_compile_features(EnigmaUnitTests PUBLIC cxx_std_11 c_std_99)
target_compile_options(EnigmaUnitTests PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
target_compile_options(EnigmaUnitTests
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${ENIGMA_CXX_COMPILE_OPTIONS}>
        ${ENIGMA_COMPILE_OPTIONS})

target_include_directories(EnigmaUnitTests PRIVATE "${GOOGLE_TEST_INCLUDE_DIR}")

target_link_libraries(EnigmaUnitTests Enigma gtest Threads::Threads)

add_test(
    NAME Units
    COMMAND EnigmaUnitTests)
