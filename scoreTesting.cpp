
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cfloat>
#include <cinttypes>
#include "scoreTesting.hpp"

extern "C" {
#include "result.h"
#include "cipher.h"
#include "scoreBasic.h"
#include "scoreNoInterleave.h"
#include "x86\cipherSsse3.h"
#include "x86\scoreSsse3.h"
#include "x86\scoreAvx.h"
}

template<typename TRet>
using  Scoring_f = TRet( const Key* const restrict key, scoreLength_t len );

namespace Enigma
{

    /** \brief Represents what is needed to test given implementation.
     */
    class ScoringParams {
        static ScoringParams ParamsForScoringOptimized;
        static ScoringParams ParamsForScoringSsse3;
        static ScoringParams ParamsForScoringOptNoInterleave;
        static ScoringParams ParamsForScoringAvx;
    public:
        enigma_score_function_t* const scoreImpl;
        union ScoringDecodedMessage* const decodedMessage;
        ScoringParams( enigma_score_function_t* scoreFunction, union ScoringDecodedMessage* decodedMessage ) :
            scoreImpl     ( scoreFunction ),
            decodedMessage( decodedMessage ) {
        }

        static ScoringParams* SelectScoringParams( EnigmaScoreFunctions_t esf ){
            switch( esf ){
            case EnigmaSF_Avx:
                return &ParamsForScoringAvx;
            case EnigmaSF_Optimized:
                return &ParamsForScoringOptimized;
            case EnigmaSF_OptNoInterleave:
                return &ParamsForScoringOptNoInterleave;
            case EnigmaSF_SSSE3:
                return &ParamsForScoringSsse3;
            case EnigmaSF_Original:
            case EnigmaSF_Simple:
            case EnigmaSF_Default:
                ;
            }
            exit ( 4 );
        }
    };

    ScoringParams ScoringParams::ParamsForScoringOptimized         ( &enigmaScoreBasic,            &decodedMsgPartBasic );
    ScoringParams ScoringParams::ParamsForScoringSsse3             ( &enigmaScoreSsse3,            &decodedMsgPartSsse3 );
    ScoringParams ScoringParams::ParamsForScoringOptNoInterleave   ( &enigmaScoreOptNoInterleave,  &decodedMsgPartNoInterleave );
    ScoringParams ScoringParams::ParamsForScoringAvx               ( &enigmaScoreAvx,              &decodedMsgPartAvx );

    /** \brief Function needed to differentiate between printing int and double.
     *
     * \param stream FILE* Output stream.
     * \param printMe TPrint Printed value.
     * \return int
     *
     */
    template<typename TPrint>
    int PrintScore( FILE* stream, TPrint printMe );

    template<>
    int PrintScore<int>( FILE* stream, int printMe ) {
        return fprintf( stream, "%i", printMe );
    }

    union dbLn {
        double db;
        int64_t ln;
    };

    template<>
    int PrintScore<double>( FILE* stream, double printMe ) {
        dbLn fnord = {printMe};
        return fprintf( stream, "%.*f [%" PRIX64 "]", DBL_DIG, printMe, fnord.ln );
    }

    /** \brief Function to allow simple compare doubles for _exact_ equality.
     *
     * \param a TCmp
     * \param b TCmp
     * \return bool Returns true if values are bit to bit exact.
     *
     */
    template<typename TCmp>
    bool AreEqual( TCmp a, TCmp b );

    template<>
    bool AreEqual<int>( int a, int b ) {
        return a == b;
    }

    template<>
    bool AreEqual<double>( double a, double b ) {
        dbLn a1 = {a};
        dbLn b1 = {b};
        return a1.ln == b1.ln;
    }

    class SimpleTestingStrategy {
        ScoringParams*          reference;
        ScoringParams*          tested;
    public:
        SimpleTestingStrategy () {
        }

        SimpleTestingStrategy( EnigmaScoreFunctions_t reference, EnigmaScoreFunctions_t tested ) :
            reference(  ScoringParams::SelectScoringParams( reference ) ),
            tested(     ScoringParams::SelectScoringParams( tested ) ) {
        }

        template < typename TScore >
        TScore TestScore(
            Scoring_f<TScore>* enigma_score_function_t::* scoreFunction,
            const Key* const                            key,
            scoreLength_t                               len ) const {
            TScore referenceScore = ( reference->scoreImpl->*scoreFunction )( key, len );
            TScore score          = ( tested   ->scoreImpl->*scoreFunction )( key, len );
            if( !AreEqual( score, referenceScore ) ) {
                FILE* scoreTestingLog = fopen( "scoreTesting.log", "a" );
                PrintDifferenceInfo( scoreTestingLog, score, referenceScore, key, len );
                fclose( scoreTestingLog );
                exit( 3 );
            }
            return referenceScore;
        }

        template < typename TScore >
        void PrintDifferenceInfo( FILE* output, TScore score, TScore referenceScore, const Key* const key, int len ) const;

        void SetScoringParams( EnigmaScoreFunctions_t reference, EnigmaScoreFunctions_t tested ) {
            this->reference = ScoringParams::SelectScoringParams( reference );
            this->tested    = ScoringParams::SelectScoringParams( tested );
        }
    };

    template < typename TScore >
    void SimpleTestingStrategy
    ::PrintDifferenceInfo( FILE* output, TScore score, TScore referenceScore, const Key* const key, int len ) const {
        print_key( output, key );
        PrintScore( output, referenceScore );
        fputs( "\n", output );
        PrintScore( output, score );
        fputs( "\n", output );
        // Standard scoring and its derivatives don't leave decodedMessage filled.
        if( reference == ScoringParams::SelectScoringParams( EnigmaSF_Optimized ) ) {
            DecodeScoredMessagePartStandard( key, len, reference->decodedMessage );
        }
        PrintDecodedMessage( output, reference->decodedMessage, len );
        fputs( "\n", output );
        PrintDecodedMessage( output, tested->decodedMessage, len );
        fputs( "\n", output );
        char compared[len + 1];
        memset( compared, ' ', len );
        compared[len] = 0;
        if( GetDifferences( reference->decodedMessage, tested->decodedMessage, compared, len ) ) {
            fputs( compared, output );
            fputs( "\n", output );
        }
    }

    template <  typename                    TTestingStrategy,
                const TTestingStrategy&     TestingStrategy >
    class ScoreTesting {
        static enigma_score_function_t ScoreTestingFunction;

        static int GetTriscore( const Key* const key, scoreLength_t len ) {
            return TestingStrategy.TestScore( &enigma_score_function_t::triscore, key, len );
        }
        static int GetBiscore( const Key* const key, scoreLength_t len ) {
            return TestingStrategy.TestScore( &enigma_score_function_t::biscore, key, len );
        }
        static int GetUniscore( const Key* const key, scoreLength_t len ) {
            return TestingStrategy.TestScore( &enigma_score_function_t::uniscore, key, len );
        }
        static double GetIcScore( const Key* const key, scoreLength_t len ) {
            return TestingStrategy.TestScore( &enigma_score_function_t::icscore, key, len );
        }
    public:
        static enigma_score_function_t* GetScoreTestingFunction() {
            return &ScoreTestingFunction;
        }
    };

    template <  typename                    TTestingStrategy,
                const TTestingStrategy&     TestingStrategy >
    enigma_score_function_t ScoreTesting<TTestingStrategy, TestingStrategy>::ScoreTestingFunction {
        ScoreTesting<TTestingStrategy, TestingStrategy>::GetTriscore,
        ScoreTesting<TTestingStrategy, TestingStrategy>::GetBiscore,
        ScoreTesting<TTestingStrategy, TestingStrategy>::GetIcScore,
        ScoreTesting<TTestingStrategy, TestingStrategy>::GetUniscore,
    };

    SimpleTestingStrategy simpleTestingStrategyInstance;
}

/** \brief Factory method returning requested testing configuration.
 *
 */
enigma_score_function_t* enigma_score_testing_create( EnigmaScoreFunctions_t reference, EnigmaScoreFunctions_t tested ) {
    Enigma::simpleTestingStrategyInstance.SetScoringParams( reference, tested );

    return Enigma::ScoreTesting < Enigma::SimpleTestingStrategy, Enigma::simpleTestingStrategyInstance >::GetScoreTestingFunction();
}
