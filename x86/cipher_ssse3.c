#pragma GCC target ("ssse3")
#include <immintrin.h>
#include <stdbool.h>

#include "cipher_ssse3.h"

void prepare_decoder_lookup_M_H3_ssse3 (const Key *key, int len);
void prepare_decoder_lookup_ALL_ssse3 (const Key *key, int len);

enigma_cipher_function_t enigma_cipher_decoder_lookup_ssse3 = {prepare_decoder_lookup_M_H3_ssse3, prepare_decoder_lookup_ALL_ssse3};


void prepare_decoder_lookup_M_H3_ssse3 (const Key *key, int len)
{
    // TODO: This is only a stub.
}

void prepare_decoder_lookup_ALL_ssse3 (const Key *key, int len)
{
    // TODO: This is only a stub.
}
