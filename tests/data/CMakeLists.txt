add_library(EnigmaTestData "")

target_sources(EnigmaTestData
PRIVATE
    "bigraph.cur.S"
    "trigraph.AVv1.S"
    "unigraph.AVv1.S"
)

target_include_directories(EnigmaTestData
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(EnigmaTestData
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/blobs"
)
