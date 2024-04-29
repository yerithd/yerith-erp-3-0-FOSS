/*
 * yerith-erp-test-utils.cpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

#include "yerith-erp-test-utils.hpp"


#include "src/utils/yerith-erp-database.hpp"


#include <QtTest/QtTest>


YerithERPWindows *Test_YerithERPTestUtils::_allWindows(0);


void Test_YerithERPTestUtils::TEST_UTILS_truncate_database_all_tables()
{
    YerithDatabase *yerith_erp_3_0_db = YerithUtils::getDatabase();

    QVERIFY(0 != yerith_erp_3_0_db);

    YerithUtils::executer_fichier_sql
    ("yerith-erp-3-0-test_data/truncate_test_yerith_erp_3_test.sql");
}


void Test_YerithERPTestUtils::setAllWindows(YerithERPWindows *allWindows)
{
    QVERIFY(0 != allWindows);

    _allWindows = allWindows;
}


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */
