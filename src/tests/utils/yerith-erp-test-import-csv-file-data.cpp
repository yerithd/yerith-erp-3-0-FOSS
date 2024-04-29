/*
 * yerith-erp-test-import-csv-file-data.cpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

#include "yerith-erp-test-import-csv-file-data.hpp"


#include "src/yerith-erp-windows.hpp"


#include "src/imports/yerith-erp-stock-import.hpp"


#include <QtTest/QtTest>


void
Test_YerithERPTestImportCSVFileData::test_TABLE_VIEW_lister_import_test_data
(const QString &aCsvFile_IN)
{
    cleanUP();

    QString
    csvFilePath(QString("%1/yerith-erp-3-0-test_data/%2").arg
                (YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER, aCsvFile_IN));

    if (!csvFilePath.isEmpty())
    {
        bool
        importSuccess = YerithUtils::import_csv_file_content(csvFilePath,
                                                             _curCsvFileToImportContentWordList);

        QVERIFY(importSuccess == true);
    }

    YerithERPStockImport
    erpStockImport(_curCsvFileToImportContentWordList,
                   _TEST_csvContentIdxToDatabaseTableColumnInfo);




    QString
    curSqlTableImportHeaderStr;

    YerithERPDatabaseTableColumnInfo *
    curDatabaseTableColumnInfo = 0;

    QStringList
    csvHeader =
                    _curCsvFileToImportContentWordList.at(0).
                    split(YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    int
    curCsvFileColumnSize = csvHeader.size();

    YerithUtils::fillDBTableColumnNameToDBTableColumnType_TEST(YerithDatabase::
                                                               STOCKS,
                                                               _dbTableColumnToType,
                                                               _dbTableColumnToIsNotNULL);

    YerithERPDatabaseTableColumnInfo *
    anERPDatabaseTableColumnInfo = 0;

    QString
    aDBColumnName;

    for (int kTEST = 0; kTEST < curCsvFileColumnSize; ++kTEST)
    {
        aDBColumnName = csvHeader.at(kTEST).trimmed();

        anERPDatabaseTableColumnInfo =
                        new YerithERPDatabaseTableColumnInfo(aDBColumnName,
                                                             _dbTableColumnToType.value
                                                             (aDBColumnName));

        _TEST_csvContentIdxToDatabaseTableColumnInfo.insert(kTEST,
                                                            anERPDatabaseTableColumnInfo);
    }

    //  qDebug() << "++ _TEST_csvContentIdxToDatabaseTableColumnInfo: " << _TEST_csvContentIdxToDatabaseTableColumnInfo;

    int
    successImportCount = erpStockImport.import();

    QVERIFY(successImportCount > 0);

    cleanUP();
}


void Test_YerithERPTestImportCSVFileData::cleanUP()
{
    QMapIterator < int,
                 YerithERPDatabaseTableColumnInfo *
                 >_it_TEST_csvContentIdxToDatabaseTableColumnInfo
                 (_TEST_csvContentIdxToDatabaseTableColumnInfo);

    YerithERPDatabaseTableColumnInfo *anERPDatabaseTableColumnInfo = 0;

    while (_it_TEST_csvContentIdxToDatabaseTableColumnInfo.hasNext())
    {
        _it_TEST_csvContentIdxToDatabaseTableColumnInfo.next();

        anERPDatabaseTableColumnInfo =
                        _it_TEST_csvContentIdxToDatabaseTableColumnInfo.value();

        if (0 != anERPDatabaseTableColumnInfo)
        {
            delete anERPDatabaseTableColumnInfo;
        }
    }
}


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */
