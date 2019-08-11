target_sources(Enigma
PRIVATE
    "src/charmap.c"
    "src/cipher.c"
    "src/ciphertext.c"
    "src/cpu.c"
    "src/date.c"
    "src/dict.c"
    "src/dict_load.c"
    "src/hillclimb.c"
    "src/input.c"
    "src/key.c"
    "src/optimizer.c"
    "src/randomNumbers.c"
    "src/result.c"
    "src/resume_in.c"
    "src/resume_out.c"
    "src/scan.c"
    "src/score.c"
    "src/scoreBasic.c"
    "src/scoreNoInterleave.c"
    "src/scoreSimple.c"
    "src/state.c"
    "src/stecker.c"

    
    "src/x86/cipherSsse3.c"
    "src/x86/cipherAvx2.c"
    
    "src/x86/computeScoreSse2.c"
    "src/x86/computeScoreSse2Vex.c"
    "src/x86/computeScoreSsse3.c"
    "src/x86/computeScoreSsse3Vex.c"
    "src/x86/computeScoreAvx2.c"
    "src/computeScoreNoInterleave.c"
    
    "src/x86/decodeMessageSsse3"
    "src/x86/decodeMessageSsse3Vex"
    "src/x86/decodeMessageAvx2.c"

    "src/x86/scoreFunctionAvx.c"
    "src/x86/scoreFunctionAvx2.c"
    "src/x86/scoreFunctionSsse3.c"

    "src/echar/echar_delta.c"

    "src/stbrett/krah_optimizer.c"
    "src/stbrett/ve3nea_optimizer.c"
)