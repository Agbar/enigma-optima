target_sources(Enigma
PRIVATE
    "src/charmap.c"
    "src/cipher.c"
    "src/ciphertext.c"
    "src/cpu.c"
    "src/date.c"
    "src/dict.c"
    "src/hillclimb.c"
    "src/hillclimb2.c"
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
    "src/stecker.c"


    "src/x86/cipherAvx2.c"
    "src/x86/cipherSsse3.c"
    "src/x86/scoreAvx.c"
    "src/x86/scoreAvx2.c"
    "src/x86/scoreSsse3.c"

    "src/echar/echar_delta.c"
)