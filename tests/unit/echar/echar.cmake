target_sources(EnigmaUnitTests
PRIVATE
    "echar/echar.tests.cpp"
    "echar/echar.with.echar_delta.tests.cpp"
    "echar/v16_echar.with.PermutationMap_t.tests.cpp"
    "echar/v16_echar.with.v16_echar_delta.tests.cpp"
    "echar/v32_echar.with.PermutationMap_t.tests.cpp"
    "echar/v32_echar.with.v32_echar_delta.tests.cpp"

    "echar/echar_delta.tests.cpp"
    "echar/v16_echar_delta.tests.cpp"
    "echar/v32_echar_delta.tests.cpp"

    "echar/turnover.tests.cpp"
    "echar/turnover.with.echar_delta.tests.cpp"
)