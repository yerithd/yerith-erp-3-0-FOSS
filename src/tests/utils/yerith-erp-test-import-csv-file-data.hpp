/*
 * yerith-erp-test-import-csv-file-data.hpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_TEST_IMPORT_CSV_FILE_DATA_HPP_
#define YERITH_TEST_IMPORT_CSV_FILE_DATA_HPP_


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST


#include "src/tests/utils/yerith-erp-test-utils.hpp"

#include "src/imports/yerith-erp-stock-import.hpp"


class YerithERPWindows;


class Test_YerithERPTestImportCSVFileData:public QObject
{
Q_OBJECT public:

    inline Test_YerithERPTestImportCSVFileData()
    {
        _allWindows = Test_YerithERPTestUtils::getAllWindows();
    }

    void test_TABLE_VIEW_lister_import_test_data(const QString &aCsvFile_IN);

private:

    void cleanUP();

protected:

    QMap < int,
         YerithERPDatabaseTableColumnInfo *
         >_TEST_csvContentIdxToDatabaseTableColumnInfo;

    QStringList _curCsvFileToImportContentWordList;

    QMap < QString, bool >_dbTableColumnToIsNotNULL;

    QMap < QString, QString > _dbTableColumnToType;

    YerithERPWindows *_allWindows;
};


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YERITH_TEST_IMPORT_CSV_FILE_DATA_HPP_ */
