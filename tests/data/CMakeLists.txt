add_library(EnigmaTestData "")

target_sources(EnigmaTestData
PRIVATE
    "benchmark_cipher_pbnxa.S"

    "bigraph.1941.S"
    "trigraph.1941.S"

    "unigraph.AVv1.S"
    "trigraph.AVv1.S"
)

target_include_directories(EnigmaTestData
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(EnigmaTestData
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/blobs"
)
