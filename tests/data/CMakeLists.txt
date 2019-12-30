add_library(EnigmaTestData "")

target_sources(EnigmaTestData
PRIVATE
    "benchmark_cipher_pbnxa.S"
    "bigraph.cur.S"
    "trigraph.AVv1.S"
    "trigraph.cur.S"
    "unigraph.AVv1.S"
)

target_include_directories(EnigmaTestData
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(EnigmaTestData
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/blobs"
)
