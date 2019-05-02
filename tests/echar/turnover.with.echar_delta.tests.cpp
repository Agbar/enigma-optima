#include "gtest/gtest.h"

extern "C" {
    #include "echar/echar_delta.h"
    #include "echar/turnover.h"
}

TEST( echar_delta, from_turnover ){
    turnover t { 7 };
    ASSERT_EQ( make_echar_delta_turnover( t ).delta, 7 );
    t.notch = 0;
    ASSERT_EQ( make_echar_delta_turnover( t ).delta, 0 );
    t.notch = 25;
    ASSERT_EQ( make_echar_delta_turnover( t ).delta, 25 );
}

TEST( turnover__echar_delta , eq ){
    turnover t { 4 };
    echar_delta d { 4 };
    ASSERT_TRUE( turnover_eq_echar_delta( t, d ) );
}

TEST( turnover__echar_delta, sub_echar_delta ){
    turnover t;
    echar_delta d;
    {
        t.notch = 5;
        d.delta = 0;
        ASSERT_EQ( turnover_sub_echar_delta( t, d ).notch, 5 );
    }
    {
        t.notch = 0;
        d.delta = 1;
        ASSERT_EQ( turnover_sub_echar_delta( t, d ).notch, 25 );
    }
    {
        t.notch = 18;
        d.delta = 1;
        ASSERT_EQ( turnover_sub_echar_delta( t, d ).notch, 17 );
    }
}

TEST( turnover__echar_delta, select_next ){
    turnover t1 { 4 };
    turnover t2 { 8 };
    echar_delta edt;
    {
        edt.delta = 1;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 4 );
        edt.delta = 4;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 4 );
        edt.delta = 5; 
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 8 );
        edt.delta = 8;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 8 );
        edt.delta = 9;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 4 );
        edt.delta = 25;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 4 );
    }
    {
        t1.notch = 24;
        t2.notch = 25;
        edt.delta = 0;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 24 );
        edt.delta = 25;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 25 );
        edt.delta = 24;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 24 );
    }
    {
        t1.notch = 0;
        t2.notch = 25;
        edt.delta = 0;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 0 );
        edt.delta = 1;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 25 );
        edt.delta = 25;
        EXPECT_EQ( turnover_select_next( edt, t1, t2 ).notch, 25 );
    }
}