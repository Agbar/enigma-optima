cmake_minimum_required(VERSION 3.1)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
include(FindThreads)

link_directories(${GOOGLE_BENCHMARK_LIB_DIR})

add_executable(EnigmaBenchmark "")
include(sourceList.cmake)

set_target_properties(EnigmaBenchmark
    PROPERTIES OUTPUT_NAME "enigma-optima-benchmark")

target_include_directories(EnigmaBenchmark PRIVATE "${GOOGLE_BENCHMARK_INCLUDE_DIR}")

target_compile_features(EnigmaBenchmark PUBLIC cxx_std_11 c_std_99)
target_compile_options(EnigmaBenchmark PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
target_compile_options(EnigmaBenchmark 
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${ENIGMA_CXX_COMPILE_OPTIONS}>)


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
