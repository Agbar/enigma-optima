#include <gtest/gtest.h>
extern "C"{
     #include "cipher.h"
     #include "echar/echar_delta.h"
}

TEST( Step1Test , Changes_N_into_N_plus_1 )
{
    int8_t state = 0;

    for( int8_t expected = 1; expected <= 25; ++expected)
    {
        Step1( &state );
        EXPECT_EQ( expected,  state );
    }
}

TEST ( Step1Test, Changes_25_into_0 )
{
    int8_t state = 25;
    Step1 ( &state );
    EXPECT_EQ( 0, state );
}

extern "C" {
    #include "cipher_inlines.h"
}

void ExpectRings( RingsState rings, int8_t r, int8_t m, int8_t l )
{
    EXPECT_EQ( r, rings.r.delta );
    EXPECT_EQ( m, rings.m.delta );
    EXPECT_EQ( l, rings.l.delta );
}

// Coded after https://web.archive.org/web/20110719081659/http://www.eclipse.net/~dhamer/downloads/rotorpdf.zip
TEST( StepAllRingsTests, DoubleStepping_David_H_Hamer )
{
    struct RingsState rings = {};
    rings.r.delta = 14; // O
    rings.m.delta = 3; // D
    rings.l.delta = 0; // A

    // We have rings I, II, III in r, m , l slots.
    struct Turnovers_t turns = {};
    turns.r.notch = 16;
    turns.m.notch = 4;
    turns.r2 = turnover_absent();
    turns.m2 = turnover_absent();

    StepAllRings( &rings, turns );
    ExpectRings( rings, 15, 3, 0 );

    StepAllRings( &rings, turns );
    ExpectRings( rings, 16, 3, 0 );

    StepAllRings( &rings, turns );
    ExpectRings( rings, 17, 4, 0 );

    StepAllRings( &rings, turns );
    ExpectRings( rings, 18, 5, 1 );

    StepAllRings( &rings, turns );
    ExpectRings( rings, 19, 5, 1 );

    StepAllRings( &rings, turns );
    ExpectRings( rings, 20, 5, 1 );
}

TEST( GetNextTurnoverTests, TestWithDoubleStepping_David_H_Hamer)
{
    struct RingsState rings = {};
    rings.r.delta = 14; // O
    rings.m.delta = 3; // D
    rings.l.delta = 0; // A

    // We have rings I, II, III in r, m , l slots.
    struct Turnovers_t turns = {};
    turns.r.notch = 16;
    turns.m.notch = 4;
    turns.r2 = turnover_absent();
    turns.m2 = turnover_absent();
 
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ).delta );
 
    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ).delta );

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ).delta );

    StepAllRings( &rings, turns );
    EXPECT_EQ( 17,  GetNextTurnover( rings, turns ).delta );

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ).delta );

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ).delta );

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ).delta );
}

TEST( GetNextTurnoverTests, TwoNotched_R_Rotor )
{
    struct RingsState rings = {};
    rings.r.delta = 0; // O
    rings.m.delta = 13; // not on turnover position
    rings.l.delta = 0; // irrelevant

    // Ring R is of type VI, VII or VIII.
    struct Turnovers_t turns = {};
    turns.r.notch = 0;
    turns.r2.notch = 13;
    turns.m.notch = 0;    // no turnover of M/L
    turns.m2 = turnover_absent();

    EXPECT_EQ( 0, GetNextTurnover( rings, turns ).delta );

    for( int i = 1; i <= 13 ; ++i )
    {
        rings.r.delta = i;
        EXPECT_EQ( 13, GetNextTurnover( rings, turns ).delta );
    }

    for( int i = 14; i <= 25; ++i )
    {
        rings.r.delta = i;
        EXPECT_EQ( 0, GetNextTurnover( rings, turns ).delta );
    }
}
