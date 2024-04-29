/*
 * yerith-erp-test-yerith-erp-windows.cpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST


#include "yerith-erp-test-yerith-erp-table-view.hpp"


#include "src/tests/utils/yerith-erp-test-import-csv-file-data.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"


#include <QtTest/QtTest>


Test_YerithERPTableView::Test_YerithERPTableView():_yerithERPTestImportCSVFileData(0),
    _allWindows(0)
{
    _yerithERPTestImportCSVFileData = new Test_YerithERPTestImportCSVFileData;

    _allWindows = Test_YerithERPTestUtils::getAllWindows();
}


void Test_YerithERPTableView::initTestCase()
{
    Test_YerithERPTestUtils::TEST_UTILS_truncate_database_all_tables();

    _yerithERPTestImportCSVFileData->test_TABLE_VIEW_lister_import_test_data
    ("yerith_test_data_stock_1_WITH_ID_french.csv");
}


void Test_YerithERPTableView::test_TABLE_VIEW_lister_fefo()
{
    YerithTableView a_test_TableView_object;

    QMap < QString, QString > stockNameToStockID_in_out;

    YerithSqlTableModel &aQSqlStockTableModel =
                    _allWindows->getSqlTableModel_stocks();

    a_test_TableView_object.lister_FEFO(aQSqlStockTableModel,
                                        stockNameToStockID_in_out);

    qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

    int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

    QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 2);

    int stockID = stockNameToStockID_in_out.value("test_yerith_1").toInt();

    QVERIFY(stockID == 3);
}


void Test_YerithERPTableView::test_TABLE_VIEW_lister_fifo()
{
    YerithTableView a_test_TableView_object;

    QMap < QString, QString > stockNameToStockID_in_out;

    YerithSqlTableModel &aQSqlStockTableModel =
                    _allWindows->getSqlTableModel_stocks();

    a_test_TableView_object.lister_FIFO(aQSqlStockTableModel,
                                        stockNameToStockID_in_out);

    qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

    int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

    QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 2);

    int stockID = stockNameToStockID_in_out.value("test_yerith_1").toInt();

    QVERIFY(stockID == 0);
}


void Test_YerithERPTableView::test_TABLE_VIEW_lister_lifo()
{
    YerithTableView a_test_TableView_object;

    QMap < QString, QString > stockNameToStockID_in_out;

    YerithSqlTableModel &aQSqlStockTableModel =
                    _allWindows->getSqlTableModel_stocks();

    a_test_TableView_object.lister_LIFO(aQSqlStockTableModel,
                                        stockNameToStockID_in_out);

    qDebug() << "++ stockNameToStockID_in_out: " << stockNameToStockID_in_out;

    int TEST_VAR_QMAP_STOCK_RESULT_COUNT = stockNameToStockID_in_out.size();

    QVERIFY(TEST_VAR_QMAP_STOCK_RESULT_COUNT == 2);

    int stockID = stockNameToStockID_in_out.value("test_yerith_1").toInt();

    QVERIFY(stockID == 3);
}


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */
