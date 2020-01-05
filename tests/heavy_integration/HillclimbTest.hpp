#pragma once


#include "HillclimbTestBase.hpp"


template< class DictsPolicy >
class HillclimbTest
    : public HillclimbTestBase
    , DictsPolicy {

public:
    static void SetUpTestCase() {
        DictsPolicy::LoadDicts();
        LoadMessage( len );
        SetKeyRange( from, to );
    }

    static void TearDownTestCase() {
        DictsPolicy::ClearDicts();
        ClearMessage();
    }
};
