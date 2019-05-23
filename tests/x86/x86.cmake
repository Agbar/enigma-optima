target_sources(EnigmaUnitTests
PRIVATE
    "x86/computeScore.tests.cpp"
    "x86/computeScoreAvx2.tests.cpp"
    "x86/computeScoreSse2.tests.cpp"
    "x86/vectorsAvx2.tests.cpp"
)
