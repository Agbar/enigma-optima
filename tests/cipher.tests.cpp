#include <gtest/gtest.h>
#include "cipher.h"

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

#include "cipher_inlines.h"

void ExpectRings( RingsState rings, int8_t r, int8_t m, int8_t l )
{
    EXPECT_EQ( r, rings.r );
    EXPECT_EQ( m, rings.m );
    EXPECT_EQ( l, rings.l );
}

// Coded after https://web.archive.org/web/20110719081659/http://www.eclipse.net/~dhamer/downloads/rotorpdf.zip
TEST( StepAllRingsTests, DoubleStepping_David_H_Hamer )
{
    struct RingsState rings = {};
    rings.r = 14; // O
    rings.m = 3; // D
    rings.l = 0; // A

    // We have rings I, II, III in r, m , l slots.
    struct Turnovers_t turns = {};
    turns.r = 16;
    turns.m = 4;
    turns.r2 = -1;
    turns.m2 = -1;

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
    rings.r = 14; // O
    rings.m = 3; // D
    rings.l = 0; // A

    // We have rings I, II, III in r, m , l slots.
    struct Turnovers_t turns = {};
    turns.r = 16;
    turns.m = 4;
    turns.r2 = -1;
    turns.m2 = -1;

    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ));

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ));

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ));

    StepAllRings( &rings, turns );
    EXPECT_EQ( 17,  GetNextTurnover( rings, turns ));

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ));

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ));

    StepAllRings( &rings, turns );
    EXPECT_EQ( 16,  GetNextTurnover( rings, turns ));
}

TEST( GetNextTurnoverTests, TwoNotched_R_Rotor )
{
    struct RingsState rings = {};
    rings.r = 0; // O
    rings.m = 13; // not on turnover position
    rings.l = 0; // irrelevant

    // Ring R is of type VI, VII or VIII.
    struct Turnovers_t turns = {};
    turns.r = 0;
    turns.r2 = 13;
    turns.m = 0;    // no turnover of M/L
    turns.m2 = -1;

    EXPECT_EQ( 0, GetNextTurnover( rings, turns ) );

    for( int i = 1; i <= 13 ; ++i )
    {
        rings.r = i;
        EXPECT_EQ( 13, GetNextTurnover( rings, turns ) );
    }

    for( int i = 14; i <= 25; ++i )
    {
        rings.r = i;
        EXPECT_EQ( 0, GetNextTurnover( rings, turns ) );
    }
}
