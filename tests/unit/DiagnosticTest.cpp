#include"nova/Basic/DiagnosticEngine.hpp"
#include"nova/Basic/SourceLocation.hpp"
#include"nova/Basic/SourceManager.hpp"
#include"gtest/gtest.h"
namespace nova {
    TEST(DiagnosticEngineTest, ReportDiagnostic) {
        SourceManager sm;
        // error: immutable variable 'x' assigned
        sm.add_file("test.nova", "let x = 10\nx = x + 1;\n");
        DiagnosticEngine de(&sm);
        auto loc = SourceLocation::create(1,12);
        auto db = de.report(DiagnosticID::err_assign_to_immutable, loc);
        db << "x";
        db.emit();
        EXPECT_EQ(de.error_count(), 1);
    }
}