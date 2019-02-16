set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include(FindThreads)

add_executable(EnigmaUnitTests "main.cpp")
target_sources(EnigmaUnitTests
PRIVATE
    "character_encoding.tests.cpp"
    "cipher.tests.cpp"
    "GetRSeed.tests.cpp"
)

target_compile_features(EnigmaUnitTests PUBLIC cxx_std_11 c_std_99)
target_compile_options(EnigmaUnitTests PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
target_compile_options(EnigmaUnitTests
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${ENIGMA_CXX_COMPILE_OPTIONS}>
        ${ENIGMA_COMPILE_OPTIONS})

target_link_libraries(EnigmaUnitTests Enigma gtest Threads::Threads)

add_test(
    NAME Units
    COMMAND EnigmaUnitTests)
