cmake_minimum_required(VERSION 3.0.0)

project(EnigmaBenchmark LANGUAGES C CXX)

set(GOOGLE_BENCHMARK_INCLUDE_DIR "" CACHE PATH "Google Benchmark include dir")
set(GOOGLE_BENCHMARK_LIB_DIR "" CACHE PATH "Google Benchmark library dir")

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
target_link_libraries(EnigmaBenchmark Enigma benchmark Threads::Threads)

if(WIN32)
    target_link_libraries(EnigmaBenchmark shlwapi)
endif()
