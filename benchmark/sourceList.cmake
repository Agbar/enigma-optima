target_sources(EnigmaBenchmark
PRIVATE
    "main.cpp"
    "MessageAndKeyBasedFixture.cpp"
    "enigmaError.c"
    "scrambler_state.benchmark.cpp"

    # "No Inline"
    "cipherAvx_ni.c"
    "cipherAvx2_ni.c"
    "cipherSimple_ni.c"
    "cipherSse2_ni.c"
    "cipherSsse3_ni.c"

    "decoding.tests.cpp"

    # decode+score tests
    "biscore.tests.cpp"
    "triscore.tests.cpp"
    "uniscore.tests.cpp"
    "icscore.tests.cpp"

    # compute score only
    "compute_icscore.tests.cpp"
    "compute_biscore.tests.cpp"
    "compute_triscore.tests.cpp"
    "compute_uniscore.tests.cpp"

    # support
    "load/str_dict.cpp"
)