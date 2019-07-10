
#include "computeScoreNoInterleave.h"
#include "dict.h"
#include "score.h"

__attribute__ ((optimize("unroll-loops")))
uint32_t ComputeTriscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len ){
    int x, score = 0;
    uint_fast8_t i0 = echar_0_based_index( msg->plain[0] );
    uint_fast8_t i1 = echar_0_based_index( msg->plain[1] );
    for( x = 2; x < len - 3; x += 4 ) {
        uint_fast8_t i2 = echar_0_based_index( msg->plain[x] );
        score += tridict[ i0 ][ i1 ][ i2 ];
        uint_fast8_t i3 = echar_0_based_index( msg->plain[x + 1] );
        score += tridict[ i1 ][ i2 ][ i3 ];
        uint_fast8_t i4 = echar_0_based_index( msg->plain[x + 2] );
        score += tridict[ i2 ][ i3 ][ i4 ];
        uint_fast8_t i5 = echar_0_based_index( msg->plain[x + 3] );
        score += tridict[ i3 ][ i4 ][ i5 ];
        i0 = i4;
        i1 = i5;
    }
    x -= 2;
    for( ; x < len - 2; ++x ) {
        score += tridict[ echar_0_based_index( msg->plain[x] ) ]
                        [ echar_0_based_index( msg->plain[x + 1] ) ]
                        [ echar_0_based_index( msg->plain[x + 2] ) ];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
int ComputeBiscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len ) {
    int score = 0, x;
    uint_fast8_t c0 = echar_0_based_index( msg->plain[0] );
    for( x = 1; x < len - 3; x += 4 ) {
        uint_fast8_t c1 = echar_0_based_index( msg->plain[x] );
        score += bidict[ c0 ][ c1 ];
        uint_fast8_t c2 = echar_0_based_index( msg->plain[x + 1] );
        score += bidict[ c1 ][ c2 ];
        uint_fast8_t c3 = echar_0_based_index( msg->plain[x + 2] );
        score += bidict[ c2 ][ c3 ];
        uint_fast8_t c4 = echar_0_based_index( msg->plain[x + 3] );
        score += bidict[ c3 ][ c4 ];
        c0 = c4;
    }
    for( ; x < len; ++x ) {
        uint_fast8_t c1 = echar_0_based_index( msg->plain[x] );
        score += bidict[ c0 ][ c1 ];
        c0 = c1;
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
int ComputeUniscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len ){
    int score = 0, i;
    for( i = 0; i < len; i++ ) {
        score += unidict[ echar_0_based_index( msg->plain[i] ) ];
    }
    return score;
}

__attribute__ ((optimize("unroll-loops")))
uint16_t ComputeIcscoreFromDecodedMsg( const union ScoringDecodedMessage* msg, scoreLength_t len ){
    uint8_t f[32] = {0};
    int i;
    for( i = 0; i < len; i++ ) {
        f[ echar_0_based_index( msg->plain[i] ) ]++;
    }

    uint16_t sum = 0;
    for( i = 0; i < 26; i++ ) {
        sum += f[i] * ( f[i] - 1 );
    }

    return sum;
}
