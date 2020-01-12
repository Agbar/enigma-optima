#pragma once


#include "HillclimbTestBase.hpp"


template< class DictsOptPolicy >
class HillclimbTest
    : public HillclimbTestBase
    , DictsOptPolicy {

public:
    static void SetUpTestCase() {
        DictsOptPolicy::LoadDicts();
        LoadMessage( len );
        SetKeyRange( from, to );
    }

    static void TearDownTestCase() {
        DictsOptPolicy::ClearDicts();
        ClearMessage();
    }

protected:
    const stbrett_optimize_f& GetStbrettOptimizer() final {
        return DictsOptPolicy::stbrett_optimizer;
    }

    void RunFinalAssertions() final;
};
