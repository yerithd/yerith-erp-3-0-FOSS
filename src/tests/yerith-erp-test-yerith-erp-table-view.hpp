/*
 * yerith-erp-test-yerith-erp-windows.hpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_TEST_YERITH_ERP_TABLE_VIEW_WINDOWS_HPP_
#define YERITH_TEST_YERITH_ERP_TABLE_VIEW_WINDOWS_HPP_


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST


#include "src/include/yerith-erp-3-0-definition-oo-class-operators.hpp"

#include "src/tests/utils/yerith-erp-test-utils.hpp"


#include <QtCore/QObject>


class Test_YerithERPTestImportCSVFileData;

class YerithERPDatabaseTableColumnInfo;

class YerithERPWindows;


class Test_YerithERPTableView:public QObject
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS Test_YerithERPTableView();

    inline ~Test_YerithERPTableView()
    {
        delete _yerithERPTestImportCSVFileData;
    }

private slots:
    void initTestCase();

    void test_TABLE_VIEW_lister_fefo();

    void test_TABLE_VIEW_lister_fifo();

    void test_TABLE_VIEW_lister_lifo();

private:

    Test_YerithERPTestImportCSVFileData *_yerithERPTestImportCSVFileData;

    YerithERPWindows *_allWindows;
};


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YERITH_TEST_YERITH_ERP_TABLE_VIEW_WINDOWS_HPP_ */
