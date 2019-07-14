#include "stbrett/krah_optimizer.h"

#include "key.h"
#include "stecker.h"
#include "echar/echar.h"


enum Action_t { NONE,
    KZ_IK, KZ_IZ,
    IX_KI, IX_KX,
    IXKZ_IK, IXKZ_IZ,
    IXKZ_IKXZ, IXKZ_IZXK,
    RESWAP };

typedef struct {
    int s1;         /* positions of letters to be swapped */
    int s2;
    int u1;         /* positions of letters to be unswapped */
    int u2;
} Change;


void OptimizeIcscore ( struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf );
void OptimizeBiscore ( struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf );
void OptimizeTriscore( struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf );


int
stbrett_optimize_krah(
    struct echar var[26],
    struct Key* ckey,
    const int len,
    const enigma_score_function_t*  scoring )
{
    Change ch;
    OptimizeIcscore( var, ckey, len, scoring );

    int jbestscore = scoring->triscore( ckey, len ) + scoring->biscore( ckey, len );
    while (1) {
        OptimizeBiscore(  var, ckey, len, scoring );
        OptimizeTriscore( var, ckey, len, scoring );

        int a = scoring->triscore( ckey, len ) + scoring->biscore( ckey, len );
        if (a > jbestscore) {
            jbestscore = a;
        }
        else {
            break;
        }
    }

    get_stecker( ckey );
    int bestscore = scoring->triscore( ckey, len );

    int newtop = 1;
    enum Action_t action;
    while ( newtop ) {
        newtop = 0;
        action = NONE;

        /* try reswapping each self-steckered with each pair,
        * steepest ascent */
        for ( int i = 0; i < ckey->count; i += 2 ) {
            SwapStbrett( ckey, ckey->sf.map[i], ckey->sf.map[i+1] );
            for ( int k = ckey->count; k < 26; k++ ) {
                SwapStbrett( ckey, ckey->sf.map[i], ckey->sf.map[k] );
                int a = scoring->triscore( ckey, len);
                if (a > bestscore) {
                    newtop = 1;
                    action = RESWAP;
                    bestscore = a;
                    ch.u1 = i;
                    ch.u2 = i + 1;
                    ch.s1 = k;
                    ch.s2 = i;
                }
                SwapStbrett( ckey, ckey->sf.map[i], ckey->sf.map[k] );
                SwapStbrett( ckey, ckey->sf.map[i+1], ckey->sf.map[k] );
                a = scoring->triscore( ckey, len );
                if (a > bestscore) {
                    newtop = 1;
                    action = RESWAP;
                    bestscore = a;
                    ch.u1 = i;
                    ch.u2 = i + 1;
                    ch.s1 = k;
                    ch.s2 = i + 1;
                }
                SwapStbrett( ckey, ckey->sf.map[i+1], ckey->sf.map[k] );
            }
            SwapStbrett( ckey, ckey->sf.map[i], ckey->sf.map[i+1] );
        }
        if (action == RESWAP) {
            SwapStbrett( ckey, ckey->sf.map[ch.u1], ckey->sf.map[ch.u2] );
            SwapStbrett( ckey, ckey->sf.map[ch.s1], ckey->sf.map[ch.s2] );
            get_stecker( ckey);
        }
        action = NONE;
    }
    return bestscore;
}


void OptimizeIcscore( struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf ){
    int i;
    struct echar x, z;
    enum Action_t action = NONE;
    uint16_t bestic = sf->icscore( ckey, len );
    uint16_t ic;
    for( i = 0; i < 26; i++ ) {
        int k;
        for( k = i + 1; k < 26; k++ ) {
            if(  ( echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                && echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                 )
            ||
                 ( echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                && echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                 )
            ){
                SwapStbrett( ckey, var[i], var[k] );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    continue;
                }
                SwapStbrett( ckey, var[i], var[k] );
            }
            else if(    echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                    && echar_neq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
            ){
                action = NONE;
                z = ckey->stbrett.letters[ echar_0_based_index( var[k] ) ];
                SwapStbrett( ckey, var[k], z );

                SwapStbrett( ckey, var[i], var[k] );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = KZ_IK;
                }
                SwapStbrett( ckey, var[i], var[k] );

                SwapStbrett( ckey, var[i], z );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = KZ_IZ;
                }
                SwapStbrett( ckey, var[i], z );

                switch( action ) {
                case KZ_IK:
                    SwapStbrett( ckey, var[i], var[k] );
                    break;
                case KZ_IZ:
                    SwapStbrett( ckey, var[i], z );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[k], z );
                    break;
                default:
                    break;
                }
            }
            else if(    echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                    && echar_neq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
            ){
                action = NONE;
                x = ckey->stbrett.letters[ echar_0_based_index( var[i] ) ];
                SwapStbrett( ckey, var[i], x );

                SwapStbrett( ckey, var[k], var[i] );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = IX_KI;
                }
                SwapStbrett( ckey, var[k], var[i] );

                SwapStbrett( ckey, var[k], x );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = IX_KX;
                }
                SwapStbrett( ckey, var[k], x );

                switch( action ) {
                case IX_KI:
                    SwapStbrett( ckey, var[k], var[i] );
                    break;
                case IX_KX:
                    SwapStbrett( ckey, var[k], x );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[i], x );
                    break;
                default:
                    break;
                }
            }
            else if(   echar_neq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                    && echar_neq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
            ){
                action = NONE;
                x = ckey->stbrett.letters[ echar_0_based_index( var[i] ) ];
                z = ckey->stbrett.letters[ echar_0_based_index( var[k] ) ];
                SwapStbrett( ckey, var[i], x );
                SwapStbrett( ckey, var[k], z );

                SwapStbrett( ckey, var[i], var[k] );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = IXKZ_IK;
                }
                SwapStbrett( ckey, x, z );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = IXKZ_IKXZ;
                }
                SwapStbrett( ckey, x, z );
                SwapStbrett( ckey, var[i], var[k] );

                SwapStbrett( ckey, var[i], z );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = IXKZ_IZ;
                }
                SwapStbrett( ckey, x, var[k] );
                ic = sf->icscore( ckey, len );
                if( ic - bestic > DBL_EPSILON ) {
                    bestic = ic;
                    action = IXKZ_IZXK;
                }
                SwapStbrett( ckey, x, var[k] );
                SwapStbrett( ckey, var[i], z );

                switch( action ) {
                case IXKZ_IK:
                    SwapStbrett( ckey, var[i], var[k] );
                    break;
                case IXKZ_IZ:
                    SwapStbrett( ckey, var[i], z );
                    break;
                case IXKZ_IKXZ:
                    SwapStbrett( ckey, var[i], var[k] );
                    SwapStbrett( ckey, x, z );
                    break;
                case IXKZ_IZXK:
                    SwapStbrett( ckey, var[i], z );
                    SwapStbrett( ckey, x, var[k] );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[i], x );
                    SwapStbrett( ckey, var[k], z );
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void OptimizeBiscore( struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf ){
    int bestscore = sf->biscore( ckey, len );
    int i, a;
    struct echar x, z;
    enum Action_t action;
    for( i = 0; i < 26; i++ ) {
        int k;
        for( k = i + 1; k < 26; k++ ) {
            if( ( echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
               && echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                )
            ||
                ( echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
               && echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                )
            ){
                SwapStbrett( ckey, var[i], var[k] );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    continue;
                }
                SwapStbrett( ckey, var[i], var[k] );
            }
            else if(    echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                    && echar_neq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
            ){
                action = NONE;
                z = ckey->stbrett.letters[ echar_0_based_index( var[k] ) ];
                SwapStbrett( ckey, var[k], z );

                SwapStbrett( ckey, var[i], var[k] );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = KZ_IK;
                }
                SwapStbrett( ckey, var[i], var[k] );

                SwapStbrett( ckey, var[i], z );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = KZ_IZ;
                }
                SwapStbrett( ckey, var[i], z );

                switch( action ) {
                case KZ_IK:
                    SwapStbrett( ckey, var[i], var[k] );
                    break;
                case KZ_IZ:
                    SwapStbrett( ckey, var[i], z );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[k], z );
                    break;
                default:
                    break;
                }
            }
            else if(    echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                    && echar_neq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
            ){
                action = NONE;
                x = ckey->stbrett.letters[ echar_0_based_index( var[i] ) ];
                SwapStbrett( ckey, var[i], x );

                SwapStbrett( ckey, var[k], var[i] );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IX_KI;
                }
                SwapStbrett( ckey, var[k], var[i] );

                SwapStbrett( ckey, var[k], x );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IX_KX;
                }
                SwapStbrett( ckey, var[k], x );

                switch( action ) {
                case IX_KI:
                    SwapStbrett( ckey, var[k], var[i] );
                    break;
                case IX_KX:
                    SwapStbrett( ckey, var[k], x );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[i], x );
                    break;
                default:
                    break;
                }
            }
            else if(   echar_neq(var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                    && echar_neq(var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
            ){
                action = NONE;
                x = ckey->stbrett.letters[ echar_0_based_index( var[i] ) ];
                z = ckey->stbrett.letters[ echar_0_based_index( var[k] ) ];
                SwapStbrett( ckey, var[i], x );
                SwapStbrett( ckey, var[k], z );

                SwapStbrett( ckey, var[i], var[k] );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IK;
                }
                SwapStbrett( ckey, x, z );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IKXZ;
                }
                SwapStbrett( ckey, x, z );
                SwapStbrett( ckey, var[i], var[k] );

                SwapStbrett( ckey, var[i], z );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IZ;
                }
                SwapStbrett( ckey, x, var[k] );
                a = sf->biscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IZXK;
                }
                SwapStbrett( ckey, x, var[k] );
                SwapStbrett( ckey, var[i], z );

                switch( action ) {
                case IXKZ_IK:
                    SwapStbrett( ckey, var[i], var[k] );
                    break;
                case IXKZ_IZ:
                    SwapStbrett( ckey, var[i], z );
                    break;
                case IXKZ_IKXZ:
                    SwapStbrett( ckey, var[i], var[k] );
                    SwapStbrett( ckey, x, z );
                    break;
                case IXKZ_IZXK:
                    SwapStbrett( ckey, var[i], z );
                    SwapStbrett( ckey, x, var[k] );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[i], x );
                    SwapStbrett( ckey, var[k], z );
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void OptimizeTriscore( struct echar var[26], struct Key* const ckey, int len, const enigma_score_function_t* const sf ){
    int bestscore = sf->triscore( ckey, len );
    int i, a;
    struct echar x, z;
    enum Action_t action;
    for( i = 0; i < 26; i++ ) {
        int k;
        for( k = i + 1; k < 26; k++ ) {
            if( ( echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
               && echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                )
            ||
                ( echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
               && echar_eq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                )
            ){
                SwapStbrett( ckey, var[i], var[k] );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    continue;
                }
                SwapStbrett( ckey, var[i], var[k] );
            }
            else if(    echar_eq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                    && echar_neq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
            ){
                action = NONE;
                z = ckey->stbrett.letters[ echar_0_based_index( var[k] ) ];
                SwapStbrett( ckey, var[k], z );

                SwapStbrett( ckey, var[i], var[k] );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = KZ_IK;
                }
                SwapStbrett( ckey, var[i], var[k] );

                SwapStbrett( ckey, var[i], z );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = KZ_IZ;
                }
                SwapStbrett( ckey, var[i], z );

                switch( action ) {
                case KZ_IK:
                    SwapStbrett( ckey, var[i], var[k] );
                    break;
                case KZ_IZ:
                    SwapStbrett( ckey, var[i], z );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[k], z );
                    break;
                default:
                    break;
                }
            }
            else if(    echar_eq(var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
                    && echar_neq(var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
            ){
                action = NONE;
                x = ckey->stbrett.letters[ echar_0_based_index( var[i] ) ];
                SwapStbrett( ckey, var[i], x );

                SwapStbrett( ckey, var[k], var[i] );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IX_KI;
                }
                SwapStbrett( ckey, var[k], var[i] );

                SwapStbrett( ckey, var[k], x );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IX_KX;
                }
                SwapStbrett( ckey, var[k], x );

                switch( action ) {
                case IX_KI:
                    SwapStbrett( ckey, var[k], var[i] );
                    break;
                case IX_KX:
                    SwapStbrett( ckey, var[k], x );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[i], x );
                    break;
                default:
                    break;
                }
            }
            else if(   echar_neq( var[i], ckey->stbrett.letters[ echar_0_based_index( var[i] ) ] )
                    && echar_neq( var[k], ckey->stbrett.letters[ echar_0_based_index( var[k] ) ] )
            ){
                action = NONE;
                x = ckey->stbrett.letters[ echar_0_based_index( var[i] ) ];
                z = ckey->stbrett.letters[ echar_0_based_index( var[k] ) ];
                SwapStbrett( ckey, var[i], x );
                SwapStbrett( ckey, var[k], z );

                SwapStbrett( ckey, var[i], var[k] );
                a = sf->triscore( ckey,  len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IK;
                }
                SwapStbrett( ckey, x, z );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IKXZ;
                }
                SwapStbrett( ckey, x, z );
                SwapStbrett( ckey, var[i], var[k] );

                SwapStbrett( ckey, var[i], z );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IZ;
                }
                SwapStbrett( ckey, x, var[k] );
                a = sf->triscore( ckey, len );
                if( a > bestscore ) {
                    bestscore = a;
                    action = IXKZ_IZXK;
                }
                SwapStbrett( ckey, x, var[k] );
                SwapStbrett( ckey, var[i], z );

                switch( action ) {
                case IXKZ_IK:
                    SwapStbrett( ckey, var[i], var[k] );
                    break;
                case IXKZ_IZ:
                    SwapStbrett( ckey, var[i], z );
                    break;
                case IXKZ_IKXZ:
                    SwapStbrett( ckey, var[i], var[k] );
                    SwapStbrett( ckey, x, z );
                    break;
                case IXKZ_IZXK:
                    SwapStbrett( ckey, var[i], z );
                    SwapStbrett( ckey, x, var[k] );
                    break;
                case NONE:
                    SwapStbrett( ckey, var[i], x );
                    SwapStbrett( ckey, var[k], z );
                    break;
                default:
                    break;
                }
            }
        }
    }
}
